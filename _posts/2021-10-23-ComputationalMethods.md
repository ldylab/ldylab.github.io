---
layout:     post                    # 使用的布局（不需要改）
title:      数值计算：使用C++             # 标题 
subtitle:   Computational Methods Using C++  #副标题
date:       2021-10-23              # 时间
author:     Henry                      # 作者
header-img: img/post-bg-coffee.jpeg   #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - Notes
---

## 数值分析：使用C++

> 最近在阅读一些论文时发现自己将公式转化为代码的能力很差，最近也一直在学习下C++，就正好使用下C++来完成下经典数值计算中的一些例子，顺便也学习下数值计算相关内容。

### 第一章：多项式插值法

#### 1.1、多项式的概念

$$
f(x)=a_{0}+a_{1} x+a_{2} x^{2}+\cdots+a_{n} x^{n}
$$

插值的原理最简单的来说，就是给定了多少个$x - y$ 的对应关系，然后根据对应关系式子联立，然后解出其中每一个不同幂次$x$的系数大小即可：

+ 利用不同的对应关系：
  $$
  \left(x_{0}, f\left(x_{0}\right)\right),\left(x_{1}, f\left(x_{1}\right)\right),\left(x_{2}, f\left(x_{2}\right)\right),\left(x_{3}, f\left(x_{3}\right)\right)
  $$

+ 解出式子中$a_0, a_1, a_2, ..., a_n$的数值大小：
  $$
  f(x)=a_{0}+a_{1} x+a_{2} x^{2}+\cdots+a_{n} x^{n}
  $$

#### 1.2、拉格朗日插值公式

+ 实际的小例子：

  <img src="https://tva1.sinaimg.cn/large/008i3skNgy1gvpn0xedkqj618o0u0jtx02.jpg" alt="image-20211023180340069" style="zoom:12%;" />

  **手算：**

  ![image-20211023212553986](https://tva1.sinaimg.cn/large/008i3skNgy1gvpn0z5jsyj61il0sb7a302.jpg)

+ 抽象例子：

  通过n点的Lagrange插值法的公式如下：
  $$
  P(x)=\sum_{k=0}^{n} L_{n, k}(x) f\left(x_{k}\right)
  $$
  其中：
  $$
  \begin{gathered}
  L_{n, k}(x)=\prod_{i=0 \atop i \neq k}^{n} \frac{x-x_{i}}{x_{k}-x_{i}}, \quad k=0,1,2, \cdots, n \\
  =\frac{\left(x-x_{0}\right)\left(x-x_{1}\right) \cdots\left(x-x_{k-1}\right)\left(x-x_{k+1}\right) \cdots\left(x-x_{n}\right)}{\left(x_{k}-x_{0}\right)\left(x_{k}-x_{1}\right) \cdots\left(x_{k}-x_{k-1}\right)\left(x_{k}-x_{k+1}\right) \cdots\left(x_{k}-x_{n}\right)}
  \end{gathered}
  $$

+ 代码实现：

  代码的思路如下：

  ![image-20211023215100043](https://tva1.sinaimg.cn/large/008i3skNgy1gvpn10ur8oj61fp0u0gsz02.jpg)

  > **Note：**
  >
  > 结构体数组在作为参数传入函数时，是只是作为一个8bit的指针而已，并不代表原来的结构体数组；这个暂时不理解，并对编程有不小影响，要解决一下（2021/10/23）
  >


### Reference

- [1] [数值计算-电子书](https://www.jianshu.com/p/fb0cb9affe19)