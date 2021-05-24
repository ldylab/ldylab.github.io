---
layout:   post
title:   Python琐碎的知识点
subtitle:  Python问题手册
date:    2020-04-06
author:   Henry
header-img: img/post-bg-mma-5.jpg
catalog: true
tags:
  - PythonLearning
---

## Python琐碎知识点
### 列表、
列表(list)是异构的，在[]内可以有不同数据类型，如：
```python
>>> [1, 2, True, 6.5]
>>> mylist = [1, 3, True, 6.5]
```
列表是有序的，支持Python序列的运算，如下表所示：

+ **索引**：[]，列表和序列是从下标0开始；
+ **连接**：\+；
+ **重复**：\*；
+ **成员**：in，询问其是否在在列表之中；
```python
>>> list = [1, 3, True, 3.5]
>>> 1 in list
>>> True
```
+ **长度**：len(list)；
+ **切片**：\[:]，mylist[1: 3]会返回一个1到2的元素列表（不含有3）；
+ **主要要点**：对于列表的复制是采用了对于列表中元素的引用（相当于是在C语言中对于该数据地址的引用），如下例子所示：
```python
>>> my_list = [1,2,3,4]
>>> A = [my_list]*3
>>> print(A)
>>> my_list[2]=45
>>> my_list = [[1, 2, 45, 4], [1, 2, 45, 4], [1, 2, 45, 4]]
```
变量A中包含三个指向mylist的引用，当原来mylist中的一个元素发生改变是，A中三处随即也发生了改变
主要用于构建数据结构的方法：
![](https://tva1.sinaimg.cn/large/008i3skNgy1gqth381et5j31ay0iyq4i.jpg)

### range函数
range常用法：
+ range(起始值, 目标值, 间隔(默认为1))，不包含目标值，相当于是\[起始值, 目标值\)
range常常和list一起使用：
```python
>>> range(10)
range(0, 10)
>>> list(range(10))
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> range(5,10)
range(5, 10)
>>> list(range(5,10))
[5, 6, 7, 8, 9]
>>> list(range(5,10,2))
[5, 7, 9]
>>> list(range(10,1,-1))
[10, 9, 8, 7, 6, 5, 4, 3, 2]
```