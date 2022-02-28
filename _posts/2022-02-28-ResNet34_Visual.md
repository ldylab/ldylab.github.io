---
layout:     post                    # 使用的布局（不需要改）
title:      ResNet34理解及其可视化            # 标题 
subtitle:   ResNet34 understanding and its visualization  #副标题
date:       2022-02-28              # 时间
author:     Henry                      # 作者
header-img: img/MarkDown-bg.jpg   #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - DeepLearning
---

## ResNet所解决的问题：

深度学习网络在网络深度过高的时候，很容易出现梯度消失的问题。而当梯度消失时（即损失函数求导为0时），权值将不再会更新，对于网络性能也不会再有改善。

而后ResNet被提出，它利用了一个非常简单的思路，通过跳过一些网络层，而直接连接至比较初始的层，从而实现缓解梯度消失的问题。

## ResNet34结构

ResNet的主体网络结构是来自VGG-19改进：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt2z8rjc3j20u01v5thw.jpg "")

而ResNet的提出使得网络层数可以变得很高，而不同深度的网络结构基本一致（ResNet18、34与ResNet50、101、152有略微的差别），只是网络层数不一样而已，其结构对应层数如论文中所述：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt2zd7520j21wh0u0qbs.jpg "")

## ResNet34结构可视化

ResNet34输入的图片为`(224, 224, 3)`（输入图片的长、输入图片的宽、输入通道`（RGB）`），整体结构如下图所示：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt2zgy332j21e60u0wh4.jpg "")

输入的图片为`224*224`尺寸的图片，然后先经过第一层卷积`Conv1`，降维处理：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt2zkt5gej21300u0wgx.jpg "")

接着再经过一次`maxpool`池化层，继续降维：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt2zp7u53j21300u076q.jpg "")

在图片从`224→56`时（缩小了4倍），开始进行`ResNet`处理，其中`ResNet`网络实现思想如下：

先进行一轮卷积核为`kernal=3`、`padding=1`、`stride=1`的卷积，输入输出保持不变：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt2zto4kjj216e0u0gob.jpg "")

然后继续重复这一结构，并且因为输入输出的维度保持不变，最后直接相加获得具有『深层网络』和『浅层网络』的特征输出：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt2zxrm4ej21ew0u0dje.jpg "")

ResNet34按照论文中，在`Layer1`中有`3`个这样的残差结构：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt303ernxj21rc0u0n40.jpg "")

而在`Layer1→Layer2`的过程中，维度是从`56→28`，所应用在`Layer2`中的残差结构也是包含了一个降维的卷积过程：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt306jr20j21dw0u077j.jpg "")

而其中的`Shortcut`实现如下：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt309bcfij21630u0add.jpg "")

最终这一个含有降维的残差网络如下：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt30cua9hj21qi0u0tc3.jpg "")

`Layer2`实现如下：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt30gmamij21kq0u00z9.jpg "")

`Layer3`、`Layer4`和`Layer2`实现的方法一致：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt30ot1i1j21ty0lu43w.jpg "")

接着后面需要再连接一个池化层，继续降维：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt30s966sj21bx0u00ur.jpg "")

最后再经过全连接层+`softmax`获得各种物体的预测概率：

![](https://tva1.sinaimg.cn/large/e6c9d24egy1gzt30xo2cyj21800tiq61.jpg "")

ResNet34结构就完成啦~！

### Reference
- [1] [Understanding and visualizing ResNets](https://towardsdatascience.com/understanding-and-visualizing-resnets-442284831be8)
- [2] [个人ResNet基于Pytorch的实现](https://arxiv.org/abs/1910.10750)
- [3] [Blog中绘图源文件](https://arxiv.org/abs/1901.02970)