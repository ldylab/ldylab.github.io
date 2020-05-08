---
layout:     post
title:      《SLAM十四讲》第一次作业
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

1. 对于个人C++的提升，目前个人已经完成对于C++的入门学习（后面会补充个人学习笔记），在这一个阶段是对于算法的学习（算法与数据结构，后面会根据个人学习进度同步更新个人笔记），在下一阶段是完成LeetCode的刷题之路，个人会完成LeetCode教学视频与《剑指Offer》这本书与其中算法问题的解答（后面会根据个人的学习进度同步更新个人笔记），这一些个人希望在暑假前能够完成到**算法与数据结构**这一个阶段，在上学前（8月20号）能够完成对于LeetCode的刷题入门（个人十分认为算法是长期的道路），在大三时期能够用于LeetCode与Effective C++和More Effective C++书籍的学习，个人C++的基础至关重要。

2. 对于SLAM课程的学习，目前个人在学习的书籍是高翔的《SLAM十四讲》与其视频课程，个人目前的规划是观看其教学视频，阅读课本，然后完成所有其留下的课后题，这一个大约会持续到（7月15号，即为暑假前）。在暑假时期，个人会将个人所学实践与个人的项目，实体测试相关的算法效果与个人ROS的学习（计划是自行购买SLAM小车，后期改进并提供完整的方案开源），个人的实践会持续至开学前（8月20号）。同时，个人会再继续阅读相关书籍《概率机器人》《机器人感知》《计算机视觉中的多视图几何》《机器人学中的状态估计》等，个人觉得非常需要在暑假时期获得老师的指导。

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

#### 软件安装的位置

- 一般在bash可以直接安装的都是在`usr/bin`文件夹中。
- 要查找使用 `apt-get install` 安装的软件，一般会采用`dpkg -L`文件名的方式。

上次看了一篇 [《从一道网易面试题浅谈OC线程安全》](https://www.jianshu.com/p/cec2a41aa0e7) 的博客，主要内容是：

#### Linux的环境变量

环境变量一般是指在操作系统中用来指定操作系统运行环境的一些参数，比如临时文件夹位置和系统文件夹位置等。因为一些参数可以被当做系统默认的设置，如此以来，在运行某个程序的时候就不需要再指定这些参数了。比如你总是使用某个程序，在命令行中使用时，每次运行这个程序你都要进入程序所在的目录启动程序。如果你将程序所在的目录加入环境变量后，运行程序时你只需要输入程序的名称，系统就会自动搜索到你程序的位置，而不会再出现`command not found`这样的问题了。

#### 在Linux修改环境变量

- 1. 直接修改`/etc/enviroment`文件，这种方法的作用域是全局的，永久性的。打开 `etc/environment` 这一个文件，在其中修改即可。
- 2. 修改/etc/profile文件，这种方法的作用域是全局的，永久性的。这个文件不是保存环境变量信息的文件，在登录时，系统只是根据它的内容对环境变量进行设置，使用：
```shell
export PATH=$PATH:[your path1]:[your path2]:[...]  
export PATH=[your path1]:[your path2]:[...]:$PATH  
```
其中，各个`PATH`之间用冒号分隔，`$PATH`指代添加your path前的环境变量。

#### Linux目录结构

> ～/：是home目录
> /：是根目录

进入home目录是直接`cd ~/`，home目录中储存的是用户的数据，就类似于windows中的user文件夹
进入根目录是直接`cd /`，储存的是系统，类似于C盘，所有软件安装于此。

**Linux的文件结构主要如下**

Linux文件结构图
![图片](https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.linuxidc.com%2FLinux%2F2019-03%2F157194.htm&psig=AOvVaw2GPlE37VbjavZfqUhlT91U&ust=1589011637484000&source=images&cd=vfe&ved=0CAIQjRxqFwoTCID-8cH8oukCFQAAAAAdAAAAABAD)

**文件功能**
- 1. **/ - 根目录：** 每一个文件和目录都从这里开始。只有root用户具有该目录下的写权限。此目录和/root目录不同，/root目录是root用户的主目录。
- 2. **/bin - 用户二进制文件：** 包含二进制可执行文件。系统的所有用户使用的命令都设在这里，例如：ps，ls，ping，grep，cp等。
- 3. **/sbin - 系统二进制文件：** 就像/bin，/sbin同样也包含二进制可执行文件。但是，在这个目录下的linux命令通常由系统管理员使用，对系统进行维护。例如：iptables、reboot、fdisk、ifconfig、swapon命令。
- 4. **/etc - 配置文件：** 包含所有程序所需的配置文件。也包含了用于启动/停止单个程序的启动和关闭shell脚本。例如：/etc/resolv.conf、/etc/logrotate.conf。
- 5. **/dev - 设备文件：** 包含设备文件。这些包括终端设备、USB或连接到系统的任何设备。例如：/dev/tty1、/dev/usbmon0。
- 6. **/proc - 进程信息：** 包含系统进程的相关信息。这是一个虚拟的文件系统，包含有关正在运行的进程的信息。例如：/proc/{pid}目录中包含的与特定pid相关的信息。
- 7. **/var - 变量文件：** var代表变量文件。这个目录下可以找到内容可能增长的文件。这包括 - 系统日志文件（/var/log）;包和数据库文件（/var/lib）;电子邮件（/var/mail）;打印队列（/var/spool）;锁文件（/var/lock）;多次重新启动需要的临时文件（/var/tmp）。
- 8. **/tmp - 临时文件：** 包含系统和用户创建的临时文件。当系统重新启动时，这个目录下的文件都将被删除。
- 9. **/usr - 用户程序：** 包含二进制文件、库文件、文档和二级程序的源代码。
- 10. **/usr/bin中包含用户程序的二进制文件：** 如果你在/bin中找不到用户二进制文件，到/usr/bin目录看看。例如：at、awk、cc、less、scp。
- 11. **/usr/sbin中包含系统管理员的二进制文件：** 如果你在/sbin中找不到系统二进制文件，到/usr/sbin目录看看。例如：atd、cron、sshd、useradd、userdel。
- 12. **/usr/lib中包含了/usr/bin和/usr/sbin用到的库**
- 13. **/usr/local中包含了从源安装的用户程序：** 例如，当你从源安装Apache，它会在/usr/local/apache2中。
- 14. **/home - HOME目录：** 所有用户用home目录来存储他们的个人档案。例如：/home/john、/home/nikita。
- 15. **/boot - 引导加载程序文件：** 包含引导加载程序相关的文件。内核的initrd、vmlinux、grub文件位于/boot下。例如：initrd.img-2.6.32-24-generic、vmlinuz-2.6.32-24-generic
- 16. **/lib - 系统库：** 包含支持位于/bin和/sbin下的二进制文件的库文件。库文件名为 ld或lib.so.*例如：ld-2.11.1.so，libncurses.so.5.7
- 17. **/opt - 可选的附加应用程序：** opt代表opitional；包含从个别厂商的附加应用程序。附加应用程序应该安装在/opt/或者/opt/的子目录下。
- 18. **/mnt - 挂载目录：**临时安装目录，系统管理员可以挂载文件系统。
- 19. **/media - 可移动媒体设备：**用于挂载可移动设备的临时目录。挂载CD-ROM的/media/cdrom，挂载软盘驱动器的/media/floppy。
- 20. **/srv - 服务数据：**srv代表服务。包含服务器特定服务相关的数据。


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
