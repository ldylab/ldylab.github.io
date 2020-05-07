---
layout:     post
title:      <SLAM十四讲》第一次作业
subtitle:   SLAM学习
date:       2020-05-07
author:     Henry
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - SLAM
---


## 前言

自己觉得从高中开始就开打比赛，打大二时已经能够非常的感觉比赛对于个人能力已经不再能够很很大的提升了，并开始从大二上学期开始去寻找一些个人喜欢的方向开始较为较为深入的学习。我在大二上学期主要还是集中于机器人学与相关控制理论的学习，设定了二阶倒立摆的大创项目，也在Matlab中完成了对于二阶倒立摆的仿真，但是现在回想起来对于上学期还是感觉个人方向规划十分混乱，缺少对于个人未来读研与职业的规划，并希望能够在这一个学期开始把自己设定为一个研究生，开始深入课题中自我学习。

对于未来的规划，个人目前会比较偏向于SLAM方向，完全是出于个人的兴趣，在阅读相关的文献资料后，我也十分认为SLAM在未来具有巨大的市场前景（自动驾驶，AR方向个人觉得都是未来趋势），因此选择以SLAM方向作为为了个人读研与工作的方向。

个人对于自我学习的规划主要是：
1.对于个人C++的提升，目前个人已经完成对于C++的入门学习（后面会补充个人学习笔记），在这一个阶段是对于算法的学习（算法与数据结构，后面会根据个人学习进度同步更新个人笔记），在下一阶段是完成LeetCode的刷题之路，个人会完成LeetCode教学视频与《剑指Offer》这本书与其中算法问题的解答（后面会根据个人的学习进度同步更新个人笔记），这一些个人希望在暑假前能够完成到**算法与数据结构**这一个阶段，在上学前（8月20号）能够完成对于LeetCode的刷题入门（个人十分认为算法是长期的道路），在大三时期能够用于LeetCode与Effective C++和More Effective C++书籍的学习，个人C++的基础至关重要。

2.对于SLAM课程的学习，目前个人在学习的书籍是高翔的《SLAM十四讲》与其视频课程，个人目前的规划是观看其教学视频，阅读课本，然后完成所有其留下的课后题，这一个大约会持续到（7月15号，即为暑假前）。在暑假时期，个人会将个人所学实践与个人的项目，实体测试相关的算法效果与个人ROS的学习（计划是自行购买SLAM小车，后期改进并提供完整的方案开源），个人的实践会持续至开学前（8月20号）。同时，个人会再继续阅读相关书籍《概率机器人》《机器人感知》《计算机视觉中的多视图几何》《机器人学中的状态估计》等，个人觉得非常需要在暑假时期获得老师的指导。

个人会将个人学习记录于此博客，用于自我积累与备忘录。

## SLAM十四讲第一次作业

#### Ubuntu软件安装
直接安装：
```shell
sudo apt-get install {AppName}
```

实用deb文件安装
```shell
sudo dpkg -i {drop the file into terminal}
```
#### Ubuntu修改源

更改源的方法：
```shell
sudo gedit /etc/apt/sources.list 
```

常用的源
ROS源
```shell
deb http://mirrors.ustc.edu.cn/ubuntu/ xenial main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ xenial-security main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ xenial-updates main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ xenial-proposed main restricted universe multiverse
deb http://mirrors.ustc.edu.cn/ubuntu/ xenial-backports main restricted universe multiverse
deb-src http://mirrors.ustc.edu.cn/ubuntu/ xenial main restricted universe multiverse
deb-src http://mirrors.ustc.edu.cn/ubuntu/ xenial-security main restricted universe multiverse
deb-src http://mirrors.ustc.edu.cn/ubuntu/ xenial-updates main restricted universe multiverse
deb-src http://mirrors.ustc.edu.cn/ubuntu/ xenial-proposed main restricted universe multiverse
deb-src http://mirrors.ustc.edu.cn/ubuntu/ xenial-backports main restricted universe multiverse
```

VIM源
```shell
# deb cdrom:[Ubuntu 16.04 LTS _Xenial Xerus_ - Release amd64 (20160420.1)]/ xenial main restricted
deb-src http://archive.ubuntu.com/ubuntu xenial main restricted #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial main restricted
deb-src http://mirrors.aliyun.com/ubuntu/ xenial main restricted multiverse universe #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted multiverse universe #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial universe
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe
deb http://mirrors.aliyun.com/ubuntu/ xenial multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse #Added by software-properties
deb http://archive.canonical.com/ubuntu xenial partner
deb-src http://archive.canonical.com/ubuntu xenial partner
deb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted multiverse universe #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe
deb http://mirrors.aliyun.com/ubuntu/ xenial-security multiverse
```


上次看了一篇 [《从一道网易面试题浅谈OC线程安全》](https://www.jianshu.com/p/cec2a41aa0e7) 的博客，主要内容是：

作者去网易面试，面试官出了一道面试题：下面代码会发生什么问题？

```objc
@property (nonatomic, strong) NSString *target;
//....
dispatch_queue_t queue = dispatch_queue_create("parallel", DISPATCH_QUEUE_CONCURRENT);
for (int i = 0; i < 1000000 ; i++) {
    dispatch_async(queue, ^{
        self.target = [NSString stringWithFormat:@"ksddkjalkjd%d",i];
    });
}
```

答案是：会 crash。

我们来看看对`target`属性（`strong`修饰）进行赋值，相当与 MRC 中的

```
- (void)setTarget:(NSString *)target {
    if (target == _target) return;
    id pre = _target;
    [target retain];//1.先保留新值
    _target = target;//2.再进行赋值
    [pre release];//3.释放旧值
}
```

因为在 *并行队列* `DISPATCH_QUEUE_CONCURRENT` 中*异步* `dispatch_async` 对 `target`属性进行赋值，就会导致 target 已经被 `release`了，还会执行 `release`。这就是向已释放内存对象发送消息而发生 crash 。


### 但是

我敲了这段代码，执行的时候发现并不会 crash~

```objc
@property (nonatomic, strong) NSString *target;
dispatch_queue_t queue = dispatch_queue_create("parallel", DISPATCH_QUEUE_CONCURRENT);
for (int i = 0; i < 1000000 ; i++) {
    dispatch_async(queue, ^{
    	// ‘ksddkjalkjd’删除了
        self.target = [NSString stringWithFormat:@"%d",i];
    });
}
```

原因就出在对 `self.target` 赋值的字符串上。博客的最后也提到了 - *‘上述代码的字符串改短一些，就不会崩溃’*，还有 `Tagged Pointer` 这个东西。

我们将上面的代码修改下：


```objc
NSString *str = [NSString stringWithFormat:@"%d", i];
NSLog(@"%d, %s, %p", i, object_getClassName(str), str);
self.target = str;
```

输出：

```
0, NSTaggedPointerString, 0x3015
```

发现这个字符串类型是 `NSTaggedPointerString`，那我们来看看 Tagged Pointer 是什么？

### Tagged Pointer

Tagged Pointer 详细的内容可以看这里 [深入理解Tagged Pointer](http://www.infoq.com/cn/articles/deep-understanding-of-tagged-pointer)。

Tagged Pointer 是一个能够提升性能、节省内存的有趣的技术。

- Tagged Pointer 专门用来存储小的对象，例如 **NSNumber** 和 **NSDate**（后来可以存储小字符串）
- Tagged Pointer 指针的值不再是地址了，而是真正的值。所以，实际上它不再是一个对象了，它只是一个披着对象皮的普通变量而已。
- 它的内存并不存储在堆中，也不需要 malloc 和 free，所以拥有极快的读取和创建速度。




### 参考：

- [从一道网易面试题浅谈OC线程安全](https://www.jianshu.com/p/cec2a41aa0e7)

- [深入理解Tagged Pointer](http://www.infoq.com/cn/articles/deep-understanding-of-tagged-pointer)

- [【译】采用Tagged Pointer的字符串](http://www.cocoachina.com/ios/20150918/13449.html)
