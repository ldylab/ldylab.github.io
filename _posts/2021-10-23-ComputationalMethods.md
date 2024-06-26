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

  ```c++
  {% raw %}
  // C++ program for implementation of Lagrange's Interpolation
  #include <iostream>
  using namespace std;
  
  class LagrangeInterpolate{
  public:
      struct Data{
          double x, y;
      };
  
  public:
      // parm1：输入已知点
      // parm2：输入想求解的点
      // parm3: 已知道点的个数
      double Interpolate(Data f[], double x_i, int f_n){
          double result = 0;
  
          for(int i = 0; i < f_n; i++){
              double term = f[i].y; // 作为公式中的f(x_n) 参数
              for(int j = 0; j < f_n; j++){
                  if(j != i){
                      term = term * (x_i - f[j].x) / double(f[i].x - f[j].x); // 分子分母的迭代过程，对应拉格朗日的通式
                  }
              }
              result += term;
          }
          return result;
      }
  };

  // 测试函数
  int main()
  {
      LagrangeInterpolate LI;
      
      // 已经知道的点
      LagrangeInterpolate::Data f[] = {{0,2}, {1,3}, {2,12}, {5,147}};
      
      // 插值结果
      cout << LI.Interpolate(f, 3, 4) << endl;
  
      return 0;
  }
  {% endraw %}
  ```

  > **Note：**
  >
  > 结构体数组在作为参数传入函数时，是只是作为一个8bit的指针而已，并不代表原来的结构体数组如下；这个暂时不理解，并对编程有不小影响，要解决一下（2021/10/23）
  > ```c++
  > {% raw %}
  > LagrangeInterpolate::Data f[] = {{0,2}, {1,3}, {2,12}, {5,147}};
  > sizeof(f); // 为64位（4*（8 + 8））
  > sizeof(f[1]); // 为16位
  > 
  > void fun(Data f){
  >   sizeof(f); // 为一个8位的指针
  > }
  > {% endraw %}
  > ```

#### 1.3、Lagrange插值算法的公式与误差问题

Lagrange 揷值法的公式虽然能保证一定会通过所有已知的 $n+1$​ 点, 也就是说, 能通过 $\left(x_{0}, f\left(x_{0}\right)\right),\left(x_{1}, f\left(x_{1}\right)\right), \cdots,\left(x_{n}, f\left(x_{n}\right)\right)$​ 。

因此, $x_{0}, x_{1}, \cdots, x_{n}$ 的不同点原本要满足 $y=f(x)=\log _{\mathrm{e}} x$ (已知函数)的函数关系，同时也会满足Lagrange插值法的公式：

$$
P(x)=\sum_{k=0}^{n} L_{n, k}(x) f\left(x_{k}\right)
$$

也就是说, $P\left(x_{0}\right)=f\left(x_{0}\right), P\left(x_{1}\right)=f\left(x_{1}\right), \cdots, P\left(x_{n}\right)=f\left(x_{n}\right)$​，所以现实中所经过的点其实并不一定说会结果上述式子中，故其一定是存在一定误差的：

$$
e(x)=f^{(n+1)}(\xi) \frac{\left(x-x_{0}\right)\left(x-x_{1}\right) \cdots\left(x-x_{n}\right)}{(n+1) !}, x_{0}<\xi<x_{n}
$$

其结论为：

已知 $f(x)$ 或 $f^{(n+1)}\left(x_{m}\right)$ 的值, 则可使用下面式计算 $e(x)$​ 的近似值：

$$
e(x)=\left(x-x_{0}\right)\left(x-x_{1}\right) \cdots\left(x-x_{n}\right) \frac{f^{(n+1)}\left(x_{m}\right)}{(n+1) !}
$$

其中$x_{m}=\frac{x_{0}+x_{n}}{2}$​, 先决条件是 $D=x_{n}-x_{0}, D$​​ 不可太大。

#### 1.4、牛顿插值算法

### Reference

- [1] [数值计算-电子书](https://www.jianshu.com/p/fb0cb9affe19)