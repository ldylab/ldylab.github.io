---
layout:     post                    # 使用的布局（不需要改）
title:      Pytorch训练的模板化            # 标题 
subtitle:   Pytorch Template  #副标题
date:       2022-02-28              # 时间
author:     Henry                      # 作者
header-img: img/post-bg-digital-native.jpg   #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - DeepLearning
---

# Pytorch训练的模板化

在平时写一些模型的时候，很多都是一些重复性的工作，比如像是数据集的准备，各种`train`、`eval`代码，这并没有什么很大的意义，并不能让自己把所有的精力放在模型的构建上，以前自己在做相关嵌入式的开发时，基本都是基于自己所构建的一套模板，并在该模板的基础上继续拓展实现自己需要的功能即可，so拥有一套自己的Pytorch模板是非常必要的事情，可以提高开发验证的效率和写代码的幸福感~！

而自己总结的这套模板也是查看了一些开源项目整体的架构，并在[torch_base](https://github.com/ahangchen/torch_base)这个基础上进一步的拓展，再次感谢！

## 代码结构

```python
pytorch_base_template
├── checkpoints # 存放模型结构参数（.pth）和tensorBoard的可视化文件

├── data        # 定义各种用于训练测试的dataset

├── model       # 定义各种实验中的模型

├── scripts     # 各种训练，测试脚本

├── utils       # 各种工具代码

├── options.py  # 定义各种实验参数，以命令行形式传入

├── train.py    # 训练代码

├── eval.py     # 测试验证代码，并能够把结果可视化

└── README.md   # 介绍一下自己的repo
```

其中用得最多的`options`：设置训练的参数；`train.py`：训练的执行代码；`eval.py`：验证并结果可视化

### options

结构如下：

```python
# 训练和测试时，一些共用的参数

def parse_common_args(parser):
  pass

# 只给到训练的参数

def parse_train_args(parser):
  pass

# 只给到测试的参数

def parse_test_args(parser):
  pass
```

全部都采用了`parser.add_argument`方式添加参数，方便在bash等中做一些参数的调整~。

**一些关键常用的参数：**

- `--model_type`：选择需要的神经网络模型（输入的参数从`model/model_entry.py`中选择）；
- `--data_type`：选择需要的数据集（输入的参数从`data/data_entry.py`中选择）；

- `--load_model_pth`：选择需要加载的网络参数（输入为pth的路径）。

### data

数据集处理的相关代码，在平时使用时，基本数据集都是一些通用数据集（COCO、VOC、CiFar等等），这里做了一些数据集选择的入口`data_entry.py`，从这个入口中选择我们需要的数据集即可：

`data_entry.py`设计还是考虑了后续的进一步拓展，底层使用python中的字典实现：

```python
def get_dataset_by_type(args, is_train=False):
    type2data = {
        'cifar_10': Cifar10Dataset(args, is_train),
        'cifar_10_without_resize': Cifar10DatasetWithoutResize(args, is_train)
    }
    dataset = type2data[args.data_type]
    return dataset
```

如果你有更多的dataset，可以继续扩展这个字典，字典访问是O(1)的，也可以避免一堆if-else的判断。有了dataset，再用pytorch的`dataloader`接口包一下，可以支持`shuffle`，多线程加载数据，非常方便。

![image-20220228142056892](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt7e2iov0j20vz0u0mz9.jpg)

后续会将「图像增强」相关函数进一步封装一下，因为很多数据集的图像增强基本是一样的。

（PS：后期可能会再将图像`transform`这个过程先独立处理，采用离线增强的方式，我个人发现在服务器跑深度学习时，很大时候瓶颈都是出现在dataloader中的做`transform`，CPU处理太慢无法喂饱显卡，后面还是尝试把这一个过程离线化）。

## model

我们所构建的网络模型，还是和`data_entry`一样，模型也是利用字典方式，选择我们需要的模型，依然支持拓展，非常方便：

![image-20220228142036335](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt7dzpp9vj20w30u0400.jpg)

同时对于很多`model`而言，有不少的网络结构都是一致的，所以设计一些小模型在`submodules.py`方便各种`model`的调用。

![image-20220228140558947](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt73cl15mj208c05g746.jpg)

## utils

主要是一些所有训练过程中的通用代码，目前有支持tensorboard的`logger.py`、计算结果混淆矩阵及其可视化的`tools_train.py`，相关图片可视化的`viz.py`等等，后续会继续把一些常用工具集成到其中。



## train.py

在训练过程中，其基本流程对于所有模型来说都是一致的：

- 训练`loss`；
- 验证`loss`；
- 验证参数（比如准确率、mAp等等）；
- 参数的可视化（tensorboard）；
- 模型的保存。

所以把上述过程封装成为各个独立的函数如下：

```python
def train(self):
  for epoch in range(self.args.epochs):
    
    # train for one epoch

    self.train_per_epoch(epoch)
    self.val_per_epoch(epoch)
    self.val_amp_per_epoch(epoch)
    self.logger.save_curves(epoch)
    self.logger.save_check_point(self.model, epoch)
```

在整一个过程中，将模型切换到训练模式，遍历整个`train_loader`，调用`step`进行数据拆包，不同`loader`返回的数据不同，拆包方式也有差异，还需要用Variable对数据再打包一下，这些操作都独立到`step`函数里，方便单独修改；再执行模型`forward`，获取结果，调用`compute_metrics`计算`metrics`（训练中也需要观察各种指标，这些指标的计算推荐放在`utils.py`下），计算`loss`，反向传播，在每次迭代中都调用`logger`的`record`函数，记录`metrics`，在最后一个`step`，调用`gen_imgs_to_write`，将torch的数据转成图像可视化，各种可视化可以写在`viz.py`中调用图像的存储。最后根据`print_freq`，每隔一段时间打印日志方便观察。

## eval.py

最后是测试代码，测试被封装为一个Evaluator，和`trainer`也比较类似：

- `__init__`：构造函数，初始化命令行参数`args`，加载模型`model`并切换到eval模式，初始化测试集的data_loader，设置一个recorder用于统计各种评估指标；
- eval：目前`eval`主要相比于`train.py`中少了反向传播的过程，并且计算了测试集的准确率、混淆矩阵和混淆矩阵的可视化过程，后面继续按照自己的项目需求继续更新优化。
- ![image-20220228140544953](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt73fcec0j20hs0dcq3p.jpg)

### Reference
- [1] [梦里风林：Pytorch实验代码的亿些小细节](https://blog.cweihang.io/ml/ncs/torch_best_practice#utils)
- [2] [个人Pytorch模板Github](https://arxiv.org/abs/1910.10750)
- [3] [利用模板做的相关案例](https://arxiv.org/abs/1901.02970)