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
### 列表
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
## 字符串(strings)
字符串其实是一种列表（list），也是一种有序的集合，之前提到了list（列表）所有的运算方法都可以应用到字符串之中，不过其也有一些特殊的使用方法：
![](https://tva1.sinaimg.cn/large/008i3skNgy1gqumkj2xvij31mm0pitay.jpg)
```python
>>> my_name 
'David'
>>> my_name.upper() # 将所有的字母全部返回为大写
'DAVID'
>>> my_name.center(10) # 将整一个字符串变成有10个字节，并将原有的字符串居中
' David '
>>> my_name.find('v') # 寻找其中字母v的个数
2
>>> my_name.split('v') # 将字符串利用v给间隔开来
['Da', 'id']
```
**字符串与列表(list)的、元组(Tuple)区别**
+ 列表可以被修改，而字符串列表不可以被修改；
![](https://tva1.sinaimg.cn/large/008i3skNgy1gqummg0nbxj31550u0dj3.jpg)
+ 字符串也是异构类型，其与元组类型，也是不可以被修改。

## 元组
元组和字符串类型，是不可以被修改的类型，其余操作和列表（list）类似：
```python
>>> my_tuple = (2,True,4.96)
>>> my_tuple 
(2, True, 4.96)
>>> len(my_tuple) 
3
>>> my_tuple[0] 
2
>>> my_tuple * 3
(2, True, 4.96, 2, True, 4.96, 2, True, 4.96)
>>> my_tuple[0:2] 
(2, True)
```
## 集合（Set）
不可修改，不能重复，无序，异构，如下所示：
```python
>>> {3,6,"cat",4.5,False} 
{False, 4.5, 3, 6, 'cat'}
>>> my_set = {3,6,"cat",4.5,False}
>>> my_set
{False, 3, 4.5, 6, 'cat'}
```
所支持的运算（同之前的list，tuple是一致的）
![](https://tva1.sinaimg.cn/large/008i3skNgy1gqumkztr7hj31960ga0u3.jpg)
集的特殊操作，与数学集合计算中类似：
![](https://tva1.sinaimg.cn/large/008i3skNgy1gqumlkc2o9j31na0qmdi9.jpg)

## 字典
无序，异构，字典名字访问，使用方法如下：
```python
>>> dict = {'item1' : 1, 'item2' : 2, 'item3' : 3}
>>> dict
{'item1': 1, 'item2': 2, 'item3': 3}
>>> dict['item1']
1
>>> dict.get('item4', alt) # 找到'item4'对应的值，要是没有知道就返回alt的内容
```
支持的运算：
![](https://tva1.sinaimg.cn/large/008i3skNgy1gqumls8y1vj31m20aedgu.jpg)
支持的操作方法：
![](https://tva1.sinaimg.cn/large/008i3skNgy1gqumm0cuh2j31ly0lg769.jpg)

总结一下：


## for的巧妙使用
**与list的一起使用**
```python
for i in [1, 3, 7, 13]:
  print(i)
```

**次数的快速实现**
```python
for i in range(5): # 一共要循环5次，其中i是从0开始，在4停止
  print(i)
```

**列表解析式**
使用for和列表可以创建非常简单的行内列表生成
sqlist = [得到x之后的运算 for x in range(~) 筛选的条件] 
相当于后面按照顺序运行之后得到的值，回到前面运算。
```python
>>> sqlist = [x*x for x in range(1, 11)]
[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
>>> sqlist = [x*x for x in range(1, 11) if x%2 != 0]
[1, 9, 25, 49, 81]
>>> [ch.upper() for ch in 'comprehension' if ch not in 'aeiou']
['C', 'M', 'P', 'R', 'H', 'N', 'S', 'N']
```

## try和raise去处理异常
try的使用方法：
```python
try:
  # 可能会出现的错误
expect:
  # 如果真的遇到问题，就执行expect中的内容
  # 然后执行接下来的内容
```
raise返回一个错误提示，raise的使用方法：
```python
if test < 0:
  raise RuntimeError("提示的内容")
```

## 类（class）使用的细节与扩展
**改写原有class的操作函数（我也不知道这样叫对不对）**
可以通过改写原有类中的函数去相加和对比，比如想要去对两个类直接相加（直接采用"+"），这一个是不可以的，不过可以通过修改class中自己所拥有的函数（__add__）函数来实现，可以调用+号，有点类似C++中类对于operation的操作：
例如：
```python
def __add__(self, other_fraction):
    new_num = self.num * other_fraction.den + self.den * other_fraction.num 
    new_den = self.den * other_fraction.den 
    common = gcd(new_num, new_den) 
    return Fraction(new_num // common, new_den // common)

# 然后假如两个class采用+相加时，就可以直接实现
>>> f1 = Fraction(1, 4)
>>> f2 = Fraction(1, 2)
>>> f3 = f1 + f2 # 成立
>>> 3/4
```
同样的该方法也是适用于其中的__eq__、__le__等等函数。

**类的继承**

可以选择继承父类中的所有参数和函数，默认是全部继承，但是假如在init中改写了一点，就相当于整一个init已经全部被改写，其继承的参数也是被改写掉，也可以使用super参数实现对其保留之下的改写：
```python
class Father(object):
    def __init__(self, name):
        self.name = name
        print("name: %s" % (self.name))

    def getName(self):
        return 'Father ' + self.name


class Son(Father):
    def __init__(self, name):
        print("hi")
        self.name = name

    # 也可以采用继承父类并修改的方法：super
    def __init(self, name):
        super().__init__(name) # 保留原来父类中的名字
        self.gender = 'boy' # 并且可以增加变量

    def getName(self):
        return 'Son ' + self.name
```
**Notes:**
+ 情况一：子类需要自动调用父类的方法：子类不重写__init__()方法，实例化子类后，会自动调用父类的__init__()的方法。

+ 情况二：子类不需要自动调用父类的方法：子类重写__init__()方法，实例化子类后，将不会自动调用父类的__init__()的方法。

+ 情况三：子类重写__init__()方法又需要调用父类的方法：使用super关键词：
```python
super(子类，self).__init__(参数1，参数2，....)
class Son(Father):
    def __init__(self, name):   
      super(Son, self).__init__(name)
```