---
layout:     post                    # 使用的布局（不需要改）
title:      机器人学的相关与思考              # 标题 
subtitle:   Robotics Learning  #副标题
date:       2021-07-29              # 时间
author:     Henry                      # 作者
header-img: img/post-bg-universe.jpg    #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - Technology
---

### 机器人学基础（运动学）

#### 位姿表示法

##### 空间的表示

+ 笛卡尔坐标系：描述空间位置、速度、加速度；
+ rgb（红绿蓝）对应xyz；
+ 描述旋转：右手定则，大拇指正向Z轴；
+ <img src="https://tva1.sinaimg.cn/large/008i3skNgy1gsxo4i57q3j30os0r83zr.jpg" alt="image-20210728113001446" style="zoom: 25%;" />
+ 坐标轴描述：frame{A}
+ 提到任意需要指明参照系的量时，我们**将坐标系写在左上角**，如坐标系{A}中点P的位置向量写作\$A_p\$​
+ 我们**将物体/点的名称写在右下角**，如坐标系{A}中点O的位置向量写作\$A_pO\$
+ 提到一个量是相对于某个坐标系时，比如{A}，我们说**with respect to** {A}，或简写为**w.r.t** {A}
+ 所使用的向量表示为**列向量**

#### 位置的变换

+ 都是从左上头开始，到右下角；

+ \${ }^{B} \overrightarrow{\boldsymbol{p}}\$ ：表示的是在B坐标系下，P向量的表示方法；

+ \${ }^{A} \overrightarrow{\boldsymbol{p}}_{O_{B}}\$：表示的是在A坐标系下的远点，移动到另一个位置，形成B坐标系；

  ![image-20210728124252079](https://tva1.sinaimg.cn/large/008i3skNgy1gsxo4mmnalj30e0089q2y.jpg)

+ 坐标的连续变换（相当于具有中间一个跳点）：

  ![image-20210728124534844](https://tva1.sinaimg.cn/large/008i3skNgy1gsxo4pvdmvj30dw09u0sw.jpg)

#### 角度的旋转

角度的旋转其实和线性代数中对于“基”的变换有很大的类似性，就相当于原来的XYZ轴，构建了一个新的基然后做变换即可。

+ 旋转矩阵：
  \$\$
  { }_{\mathrm{B}}^{\mathrm{A}} \mathrm{R}=\left[\begin{array}{lll}
  { }^{A} \overrightarrow{\boldsymbol{X}}_{B} & { }^{A} \overrightarrow{\boldsymbol{Y}}_{B} & { }^{A} \overrightarrow{\boldsymbol{Z}}_{\boldsymbol{B}}
  \end{array}\right]
  \$\$

+ 在图像上的意义为：

  ![image-20210728125853161](https://tva1.sinaimg.cn/large/008i3skNgy1gsxo4t9yygj30w00l2jsi.jpg)

+ 转置（相当于是从A->B，变成了从B->A的转化）

  \$\$
  { }_{\mathrm{B}}^{\mathrm{A}} \mathrm{R}^{\boldsymbol{T}}=\left[\begin{array}{c}
  { }^{A} \overrightarrow{\boldsymbol{X}}_{\boldsymbol{B}}^{T} \\
  A \overrightarrow{\boldsymbol{Y}}_{B}^{T} \\
  { }^{A} \overrightarrow{\boldsymbol{Z}}_{\boldsymbol{B}}^{T}
  \end{array}\right]
  \$\$

+ 相关性质：点积为单位向量

  ![image-20210728130542143](https://tva1.sinaimg.cn/large/008i3skNgy1gsxogry4ngj30ed09n0t9.jpg)

+ 旋转的式子拓展：

  ![image-20210728130711381](https://tva1.sinaimg.cn/large/008i3skNgy1gsxogvmt9ej30b1036dfr.jpg)

+ 欧拉角、转轴转角、四元数

  + 转轴转角：相当于是绕从原点的开始的某一个向量做旋转一定的角度：

    ![image-20210728131139678](https://tva1.sinaimg.cn/large/008i3skNgy1gsxogyyn9bj30ah08o0sw.jpg)

  + 欧拉角或是四元数就是采用按照一定的顺序绕某一个轴旋转：

    ![image-20210728131247351](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoh1rhfbj30ex0a6glt.jpg)

    在飞控上用的很多：

    ![image-20210728133002043](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoh5hrk1j30jp0nijt1.jpg)

  + 变换矩阵（4*4的矩阵）

    ![image-20210728132636184](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoh9qhvqj31120kaab9.jpg)

  + 计算的顺序要记得：
    \$\$
    \begin{aligned}
    { }^{A} \boldsymbol{p} &={ }_{B}^{A} T^{B} \boldsymbol{p} \\
    { }_{C}^{A} T &={ }_{B}^{A} T_{C}^{B} T
    \end{aligned}
    \$\$

+ **万向节死锁**

  + 我们发现，在第二个关节转了90度之后，第三个关节不管怎么转，其实和第一个关节的转轴是一样的！这个end effector在此刻失去了绕外在坐标的y轴旋转的能力。图上只画了θ1为0°的情况，但很容易想象，不管\$\theta_1\$是多少，只要\$\theta_2\$​，这样的情况都必然发生。

  + ![image-20210728133346430](https://tva1.sinaimg.cn/large/008i3skNgy1gsxohedidjj30fe0et74h.jpg)

  + 只要第二个关节为90度，其他两个关节无论如何做工都是只能在Y轴上运动而已。

  + 万向轴锁

    [![img](https://tva1.sinaimg.cn/large/008i3skNgy1gsxohimpkyg30dc07ix6f.gif)](https://www.guyuehome.com/Uploads/wp/2020/04/IMG_1483.gif)

#### DH建模

+ 机器人关节的表示：

  ![image-20210728140536495](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoiaqlhej30dm05zgll.jpg)

+ 论文常见的单词表示：

  + operational space（操作空间）
  + redundancy（冗余自由度）
  + obstacle avoidance（避障）
  + self-collision avoidance（避免自己撞到自己）
  + Joint limit（关节可运动范围的限制）
  + singularity（奇异点）
  + end effector position and orientation（末端位姿）
  + inverse kinematics和forward kinematics
  + frame attachment（坐标系确定）
  + convention（惯例）
  + link length（连杆长度）
  + link twist（连杆扭转）
  + ***link offset*****（连杆偏移）**
  + ***joint angle*****（关节转角）**
  + **轴（axis）**
  + **公共法线（common normal）**

+ **奇异点：**像这种机械臂的end effector失去了某个方向的自由度，在该方向上运动速度只能为0、该方向上的力/扭矩全完全由机械结构承担（而不需要关节上的电机/肌肉等actuator出力）的情况，就称为kinematic singularity。

  ![image-20210728142200078](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoiffuh9j31au0m8q49.jpg)

+ DH建模，分为：SDH和MDH两种，后者是有所改进的version，我觉得还是用例子就可以啦，在很多机器人工程里面，执行器大多数为周转关节Revolute Joint，我个人基本很少见到平移关节（在龙门结构中见得比较多），而对于关节设置，绝大多数机器人在连杆位置并不会和题目一样做（类似像麻花一样被扭得奇奇怪怪的形状），而是在数学表达上只是简单的左平移而已，对于使用DH方法建模非常简单，建模的例子为链式**机械臂：**

  + **DH参数与坐标系确定(frame attachment)**  **DH参数就是一个用四个参数表达两对关节连杆之间位置角度关系的机械臂数学模型和坐标系确定系统**。

  + ![image-20210728151313925](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoim91wtj30i00cnt8y.jpg)

  + 最有用的一张图，比国内各种写得很复杂的要更加直观

    ![image-20210728151413652](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoipc0bgj30e304c0t3.jpg)

  + 根据机器人实际各个关节长度做好标记

    ![image-20210728151458343](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoitwypjj30hz0buwev.jpg)

  + 按照如下表格做好列表即可：

    ![image-20210728151532963](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoiy6a7vj30i704d0su.jpg)

#### 正运动学

正运动其实就是关节坐标的传递计算即可，并不是很复杂，在上述获得了DH参数后，就是相当于是获得了在\$frame_A\$->\$frame_B\$->\$frame_C\$​之间的传递信息，按照线性代数直接相乘即可：对于其中每一个角度都是已经定好的，直接代入即可

![image-20210728152236509](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoj11nbvj30h406iwes.jpg)

#### 雅克比矩阵

雅克比矩阵的直观理解：求微分其实就是线性化，导数其实就是线性空间之间的线性变换，Jaocibian矩阵本质上就是导数。在原来正运动学的基础上，加上了Jaocibian Matrix就是相当于是对于位置做了微分处理，可以实现其实时速度的求解，雅克比矩阵其实在另一方面也是对于矩阵非线性部分所做的线性化处理，也就是探究的是每一个影响结果的变量（\$x_1, x_2, ...\$微小变化（线性化限制）对于最后末端执行器运动的趋势），该部分我觉得可汗学院讲的非常好~。

+ 对于正运动学在函数上的思路理解：
  \$\$
  \overrightarrow{\boldsymbol{x}}=f(\overrightarrow{\boldsymbol{q}})
  \$\$
  其中\$q\$向量表示关节位置，\$x\$向量表示\$end effector\$的位置和朝向。

+ 对其直观的对时间做微分得：
  \$\$
  \frac{d \vec{x}}{d t}=\frac{d f(\vec{q})}{d t}=\frac{d f(\vec{q})}{d \vec{q}} \cdot \frac{d \vec{q}}{d t}
  \$\$
  即：
  \$\$
  \dot{x}=\frac{d \vec{x}}{d \vec{q}} \cdot \dot{q}
  \$\$

+ 可以写成：
  \$\$
  \dot{x}=J \dot{q}
  \$\$

+ 其中的\$J\$能写成如下，代表了第i行第j列表示的物理意义就是当第j个关节运动时，操作空间的第i个平动/转动方向会如何运动：
  \$\$
  J=\left[\begin{array}{ccc}
  \frac{d x_{1}}{d q_{1}} & \cdots & \frac{d x_{1}}{d q_{n}} \\
  \vdots & \ddots & \vdots \\
  \frac{d x_{m}}{d q_{1}} & \cdots & \frac{d x_{m}}{d q_{n}}
  \end{array}\right]
  \$\$

+ 对于机器人末端而言，其为为一个六个哥维度（位姿）的object，需要使用六个参数去表示，如下：
  \$\$
  \dot{x}=\left[\begin{array}{c}
  v_{x} \\
  v_{y} \\
  v_{z} \\
  \omega_{x} \\
  \omega_{y} \\
  \omega_{z}
  \end{array}\right]=\left[\begin{array}{l}
  \vec{v} \\
  \vec{\omega}
  \end{array}\right]
  \$\$

+ 同时其雅克比矩阵也能写成：
  \$\$
  J=\left[\begin{array}{l}
  J_{v} \\
  J_{\omega}
  \end{array}\right]
  \$\$
  其上半部分对应为线速度，下半部分代表为角速度。

+ 线速度的求解：

  + 经典系统：

    ![image-20210728154844398](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoj6jrnvj30gm0h53yx.jpg)

  + end effector位置的正运动学表达式：
    \$\$
    \begin{aligned}
    &x_{e}=\cos \left(\theta_{1}+\theta_{2}\right)+\cos \theta_{1} \\
    &y_{e}=\sin \left(\theta_{1}+\theta_{2}\right)+\sin \theta_{1}
    \end{aligned}
    \$\$

  + 对其能运动的关节做一次求导，也就是雅克比的过程：
    \$\$
    J=\left[\begin{array}{ll}
    \frac{d x_{e}}{d \theta_{1}} & \frac{d x_{e}}{d \theta_{2}} \\
    \frac{d y_{e}}{d \theta_{1}} & \frac{d y_{e}}{d \theta_{2}}
    \end{array}\right]=\left[\begin{array}{cc}
    -\sin \left(\theta_{1}+\theta_{2}\right)-\sin \theta_{1} & -\sin \left(\theta_{1}+\theta_{2}\right) \\
    \cos \left(\theta_{1}+\theta_{2}\right)+\cos \theta_{1} & \cos \left(\theta_{1}+\theta_{2}\right)
    \end{array}\right]
    \$\$

  + 对于使用DH建模方法的机器人同样也是使用，因为求取的是线速度部分，只要注意留意下\$4 \times 4\$最后一列即可：

    ![image-20210728155144346](https://tva1.sinaimg.cn/large/008i3skNgy1gsxojaex9lj30ca03xwem.jpg)

  + 雅克比求解过程：

    ![image-20210728155213267](https://tva1.sinaimg.cn/large/008i3skNgy1gsxojd1596j30dz03bmx7.jpg)

  + 角速度部分（\$J_W\$）

    其实其角度是代表了末端与\$X\$轴的夹角：

    ![image-20210728155535603](https://tva1.sinaimg.cn/large/008i3skNgy1gsxojicdwhj30gd0gedg5.jpg)

  + 因为其只有\$\alpha\$一个维度的变换，在矩阵中只有一行，表示如下：
    \$\$
    J_{\omega}=\left[\begin{array}{ll}
    \frac{d \alpha}{d \theta_{1}} & \frac{d \alpha}{d \theta_{2}}
    \end{array}\right]
    \$\$

  + 而且对于机器人系统而已，其角度能直接叠加，所以能在DH中，直接求得每一个变换矩阵之间角度变换即可，就能在最后看出每一个角度的变换对于末端的结果影响是什么样的，同时因为平移关节的运动不可能改变end effector的朝向，所以end effector的orientation对平移关节位置的求导一定是0：
    \$\$
    J_{\omega}=\left[\begin{array}{llll}
    { }^{0}  \vec{Z}_{1} & \overrightarrow{0} & { }^{0} \hat{Z}_{3} & { }^{0} \hat{Z}_{4}
    \end{array}\right]
    \$\$

  + 用于DH建模中每一个坐标系的传递：

    ![image-20210728161933162](https://tva1.sinaimg.cn/large/008i3skNgy1gsxojpa7bzj30fc05y3yq.jpg)

  + 对于不同的坐标系也是用同样的方法，其思想也都是一样的：

    + 比如对于一个柱坐标而已，其利用的是：
      \$\$
      \begin{gathered}
      x=\rho \cos \theta \\
      y=\rho \sin \theta \\
      z=z
      \end{gathered}
      \$\$

    + 那么转换矩阵Ep（这里的p表示position）就**由笛卡尔坐标对柱坐标的向量求导**得出：
      \$\$
      E_{p}=\frac{d\left[\begin{array}{l}
      x \\
      y \\
      z
      \end{array}\right]}{d\left[\begin{array}{c}
      \rho \\
      \theta \\
      z
      \end{array}\right]}=\left[\begin{array}{ccc}
      \cos \theta & \sin \theta & 0 \\
      -\rho \sin \theta & \rho \cos \theta & 0 \\
      0 & 0 & 1
      \end{array}\right]
      \$\$

  + 基本雅可比矩阵（Basic Jacobian）是用笛卡尔坐标描述操作空间**线速度**及**角速度**求得的雅可比矩阵：
    \$\$
    J=\left[\begin{array}{l}
    J_{v} \\
    J_{\omega}
    \end{array}\right]
    \$\$

  + 对于雅克比矩阵而言，但是其并非为一个方阵：
    \$\$
    J=\left[\begin{array}{ccc}
    \frac{d x_{1}}{d q_{1}} & \cdots & \frac{d x_{1}}{d q_{n}} \\
    \vdots & \ddots & \vdots \\
    \frac{d x_{m}}{d q_{1}} & \cdots & \frac{d x_{m}}{d q_{n}}
    \end{array}\right]
    \$\$
    其中雅克比矩阵有m行n列：m为操作空间的自由度（最多六轴），n为关节空间的自由度（关节的数量，非奇异点）

  + 对于雅克比矩阵而言，在位置与速度能有传递作用，对于力而言，其也是一个线性的过程，雅可比矩阵同样也是连接关节空间与操作空间力/扭矩映射关系的纽带。（这一部分不是很懂，还是需要后面动力学的继续学习）

+ 

#### 逆运动学

逆运动学在机器人控制里面扮演非常重要的角色，在很多项目任务里面都是希望能从A点运动到B点，并在其轨迹中要需要做比如躲避障碍物等一些动作，在很多书籍中采用的逆运动学都是直接类似对transport matrix直接做逆处理，但是对于一个六轴系统（普遍），其求逆过程非常耗费资源，在很多时候会对其做线性化处理，然后做各种各样的优化，在最近主流的工具或库中，也出现了很多例如Python Robotic Toolbox、V-REP等等，对于逆运动学求解优化一直是一个难点所在，也需要很多数学优化工具（降阶、梯度下降方法、非线性目标规划方法、凸优化等等），需要自己在后续做更多数学的学习与相关代码的验证与实现。

+ 逆运动学在做什么？

  **问题描述：**逆运动学问题是：已知**(xe, ye)**，怎么求**(θ1,θ2)**，就是已经知道了他的位置，要如何得到驱动关节的角度。

  ![image-20210728174209806](https://tva1.sinaimg.cn/large/008i3skNgy1gsxojxr878j30h70agaad.jpg)

+ 其中式子有两个未知数、两个方程，解方程就可以了。不过这是一个**非线性方程组**，可以两个等式都两边平方然后相加，可以求出\$\theta_2\$​，然后再求出\$\theta_1\$​​。

+ 但是很多机器人都是六轴，其我们用包含关节位置的Transformation Matrix（齐次坐标变换矩阵）来表示end effector的位置和朝向，这个矩阵包含了end effector position w.r.t {0}（一个3×1的向量），以及end effector的旋转矩阵w.r.t {0}（一个3×3的矩阵），一共12个未知数，求解一般会采用：

  + **解析法**：就是采用纯粹建模，然后逆求解的方法，通常随着自由度上升，求得解析解的难度也越来越大，对于冗余机械臂还需要从几个解中选择合适的解。这种方法通常用在特定几何结构的机械臂，并且有特定的关节位置求解顺序，其并不具有通用性。

  + 优化法，就是将问题转为一个优化问题求数值解：
    \$\$
    q=f^{-1}(x)
    \$\$
    其实最优化问题就是之前数模中线性规划与现代控制里面的最优化问题，就是求解在约束条件下一个min或是max问题，在这里为求关节位置q，使实际end effector位置x与正运动学算出的end effector位置f(q)之间的差最小”的问题：
    \$\$
    \min _{q} e, \text { where } e=(x-f(q))^{2}
    \$\$

  + 对于上述式子的求解，就变成了一个纯粹的数学问题了，一般是采用：

    + **迭代解 - 雅可比矩阵求逆法（Iterative Method - Jacobian Inverse）**，是把问题“微分化”，利用瞬时运动学的逆运算无限逼近；

      ![image-20210728211316536](https://tva1.sinaimg.cn/large/008i3skNgy1gsxok2usjsj30ht06swem.jpg)

    + **雅可比矩阵转置法（Jacobian Transpose）**，是从下面式子出发，用雅可比矩阵的转置代替困难的求逆运算：
      \$\$
      \tau=J^{T} F
      \$\$

    + 由于在逆运动学求解中，我们不关心系统的动力学特性，上面这个式子也可以写成：
      \$\$
      \dot{q}=J^{T} \dot{x}
      \$\$

    + **冗余雅可比矩阵求逆**：实在有点太难了，以后再来~

+ 

### 机器人学（初级动力学）

动力学学得太水了，留个坑，以后再来了~

### 机器人控制

我个人感觉其实只做运动学的话，对于机器人控制基本执行器（无论是有刷无刷）都最终和舵机控制没啥很大差别，机器人想要做得好还是得做基于动力学的力控会更好一些些。我在之前大龙项目中也深刻的感受到了这一个问题，并不是我的控制器方案做的不好，而是对于位置控制而已，其能达到的效果确实是有限的，无法去做精确的位置、速度、加速度控制，发生抖动也是不可避免的。

在研究机器人的过程中，因为研究的是力，所以我们对机械臂的了解，在“关节的运动如何改变末端执行器的运动”以外又深入了一层，进入了“关节驱动器（actuator）产生的力如何改变关节的运动、从而改变end effector的运动”这个层面。也因此，我们将要控制的变量，也从关节的位置或速度，变成了关节的力。如果你有准确的动力学模型，你就能够准确控制关节的加速度，从而准确控制end effector的加速度。我们知道，位置是加速度对时间的两次积分，这意味着，直接控制加速度允许你对轨迹做更精确快速的跟踪。**动力学的引入允许我们更快、更精确地跟随需要的轨迹**。**动力学的引入让我们有可能控制机械臂与外界的相互作用力。**

#### 单关节位置控制

大部分时候我们最终关心的都是**末端执行器**：往汽车外壳喷涂油漆，我们需要控制末端的**运动轨迹（Motion Trajectory）**，即每一时刻的**位置/姿态、速度、加速度（Position / Orientation, Velocity, Acceleration）** **末端的运动轨迹、力/力矩、阻抗/导纳，这三者是我们控制机械臂的最终目标**，目标的选择由不同的应用场景决定。(对这三者，*我们之后简称位置或者运动、力、和阻抗*）。 在机械臂上，我们无法直接操控末端，而**只能通过控制机械臂上的每一个关节去改变末端的运动和施力情况**。在每一个关节上，我们同样有**运动轨迹****、扭矩和阻抗**这三个控制对象的选项。能够分别对每一个关节的运动轨迹、扭矩或者阻抗进行控制，是控制末端执行器的基本要求。 然而串联机械臂的每一个关节并不是完全独立的，通过前面对机械臂动力学模型的分析我们知道，每一个关节的运动和输出的力都可能对其它关节的运动和受力产生影响，也就是动力学耦合（Dynamic Coupling）现象。如果对每一个关节单独进行控制，动力学耦合的存在会大大降低每个控制器的控制性能（响应速度、频宽等等）。 如果我们把所有关节的状态（运动或扭矩）看作一体，在**“关节空间”（Joint Space）**上进行控制，那么我们就有可能把机械臂的动力学模型加入我们的控制回路中，实现**“解耦”（decoupling）**，从而提升机械臂控制器的动态性能。 闭环控制必然需要传感器。在机械臂上，最常见的传感器是**关节位置传感器**、**关节扭矩传感器、以及末端的力/力矩传感器**。末端的实际位置/姿态只有外部传感器才有可能直接测量，其更新速率通常无法用来做闭环控制；但**通过运动学计算，也可以比较准确地从关节位置得到末端位置**。阻抗表述的是力与位置的动态关系，只能由力传感器和位置传感器间接测定。 下面这幅图标出了在关节和末端上我们可以控制的目标，加粗字体表明这个量能够用传感器直接测量输出。

![image-20210728213728008](https://tva1.sinaimg.cn/large/008i3skNgy1gsxok8jpgoj30lc0fdwf2.jpg)

 根据控制目标和对象的不同，机械臂的控制有许多种不同的方法、形成了许多条**从控制对象到控制目标的“路径”**。比如说，控制末端的位置（控制目标），我们既可以通过控制关节的位置（控制对象）来实现，也可以通过控制关节的力矩（控制对象）来实现；控制末端的阻抗同样也能通过关节位置或者关节力矩来实现。

对于电机的控制，在工业上一般会采用直流电机或是采用无刷电机去作为机器人执行器，一般经过减速传动装置控制关节运动。这个时候要控制关节的位置，我们就需要通过调节电机输出的扭矩来实现，对于机器人而言，电机就是机器人其中的“拉力发生器”，关节则是那个“方块“，（减速装置其实也具备弹簧+阻尼的性质，但我们这里忽略不计）；这时候闭环控制必不可少。电机运动的原理是通电导线在磁场中受到力而运动：这个力的大小和电流的大小和该处磁场的大小和方向有关（复习一下安培力）。电力拖动课程中，说到无论是有刷电机还是无刷电机，**电机输出的转矩与输入它的电流是成正比的**，它们的比叫电机的torque constant（转矩常数kt）:
\$\$
\tau=k_{t} \cdot i
\$\$
因此，通过控制电机的电流，我们就能控制电机输出端的扭矩（这也是一个开环控制，但是是一个比较不会有误差的开环控制）。由于市场上有许多封装好的驱动帮你隐藏了这些细节，可能你很少需要关注到电流控制这个层面。对无刷电机而言，电流控制并不像说起来那么简单——驱动器需要控制每个相的电流按照一定的规律变化才能让电机转起来并且输出与指令一致的扭矩。关于无刷电机的控制我们以后再聊。 电机输出的扭矩经过减速装置放大后使关节产生运动，编码器则反馈关节位置给控制器以生成合适的电机扭矩/电流指令——我们把这个控制回路套到我们前面讲的控制框图里（θ是关节位置）：

![image-20210728214148931](https://tva1.sinaimg.cn/large/008i3skNgy1gsxokc91vfj30jv072weq.jpg)

这里的运动控制一直说的“位置控制”，更准确地说应该是“运动控制”，关节的位置、速度、加速度都应在控制范围之内。 弄清楚了这个控制回路以后，控制器可以根据需要选择。我们用一个简单的PD控制器来举例，则有：（作为控制器的输出，其中的位置和速度是传感器可获得）
\$\$
\tau=k_{p}\left(\theta_{d}-\theta\right)+k_{d}\left(\dot{\theta}_{d}-\dot{\theta}\right)
\$\$
是关注到了其中角度的变化与角速度的变化过程，就是P（位置）+D（位置微分=速度）的控制。

而对于电机控制而言，其对于一个关节做动力学分析可得：（被控对象，二阶系统）
\$\$
\tau=m \ddot{\theta}+b \dot{\theta}
\$\$
可以看到，关节的动力学方程由正比于加速度的惯性力和正比于速度的粘滞阻力组成。 结合控制器的输出与系统的动力学方程，我们可以写出这个控制回路的时域方程：
\$\$
k_{p}\left(\theta_{d}-\theta\right)+k_{d}\left(\dot{\theta}_{d}-\dot{\theta}\right)=m \ddot{\theta}+b \dot{\theta}
\$\$
所以可以看到其被控对象就是一个二阶系统，可以用二阶系统相关的控制理论去分析调试。

#### 单关节阻抗控制（找时间用Innfos做一下测试）

+ 电路与机械阻抗类比：

  ![image-20210728220930663](https://tva1.sinaimg.cn/large/008i3skNgy1gsxokgwpt5j30f70a1gm4.jpg)

+ 阻抗控制的直观理解：

  比起去分别设定“符合物理规律”的力和位置的控制目标，我们不如去设定能够决定位置/速度/加速度如何随着力改变的量、或者对外输出的力如何随着位置/速度/加速度变化的量，也即“阻抗”/“导纳”。比如说，在方块弹簧的例子中，我可以设定这样一个小目标：*当我迎面撞上这个方块时，我好像撞到了一块软软的海绵上——*即使它连着的弹簧其实很硬。

+ 阻抗控制两种控制思路：

  + **根据当前的运动状态，以及目标阻抗，决定输出的力**

    ![image-20210728221202557](https://tva1.sinaimg.cn/large/008i3skNgy1gsxokk7vf8j30jv096glv.jpg)

    这里外环是阻抗环，内环是力矩环，有时候力矩传感器会被省掉，直接用电流做开环力矩控制。

  + ![image-20210728221322413](https://tva1.sinaimg.cn/large/008i3skNgy1gsxoknc7wwj30i3091q34.jpg)

    这里外环是阻抗环，内环是位置环（运动控制环），位置环我们通常都会做闭环控制。

  + **基于内环力控制的阻抗控制**

    + 如果阻抗控制器不存在的话，那么系统的响应将会遵循下面这个动力学方程，其中\$f_{ext}\$​表示外界对系统施加的力（也就是你撞上去的力），并且假设弹簧的原点是0：
      \$\$
      f_{\text {ext }}=m \ddot{x}+b \dot{x}+k x
      \$\$

    + 而阻抗控制的目标则是要把系统响应变成下面这样的动力学方程，其中{md, bd, kd}即是我们希望系统对外表现出的惯性、阻尼、刚性特征（仍然假设我们的位置原点是0）：
      \$\$
      f_{d}+f_{e x t}=m_{d} \ddot{x}+b_{d} \dot{x}+k_{d} x
      \$\$

    + 从上面两个式子，我们可以得到阻抗控制器需要输出的力f_d为:
      \$\$
      f_{d}=\left(m_{d}-m\right) \ddot{x}+\left(b_{d}-b\right) \dot{x}+\left(k_{d}-k\right) x
      \$\$

    + 在这个方法里，我们需要知道系统本身的动力学模型{k, b, m}，才能准确计算出所需的力，有时候我们也称之为**基于模型的阻抗控制（model-based impedance control)**。另外，用这个方法也很难让系统呈现比较大的刚性（设一个很大的\$k_d\$）——当\$k_d\$​很大时，微小的测量位置变化即会引起很大的力指令变化，系统会非常容易不稳定。

    + 通常情况下，我们可以不对系统呈现的“惯量”进行控制。另外，上述的分析为了简化等式假设参考的位置/速度都为0。如果我们忽略惯量/加速度项，并把理想的方块位置原点记为x_d，阻抗控制可以写作如下形式：
      \$\$
      f_{d}=\left(b_{d}-b\right)\left(\dot{x}-\dot{x}_{d}\right)+\left(k_{d}-k\right)\left({x}-{x}_{d}\right)
      \$\$

    + 这个式子与关节位置控制所用的PD控制的式子是非常相似的。在基于运动学的关节位置控制中，我们通常会选取适当的P和D的控制系数，**使得系统在稳定的前提下刚性尽可能的高，以实现快速精确的轨迹跟踪**。而在阻抗控制中，为了实现接触的柔性（比较低的k_d，也即比较低的P gain），则需要以牺牲轨迹跟踪的精度和速度为前提。

  + **基于内环运动控制的阻抗控制**

    + 现在同样的控制目标用第二种方法来实现：当你撞上方块，它会测量到方块的受力，并计算出理想的运动轨迹使你感觉好像撞在海绵上。这时候，我们依然希望系统的状态方程如下（仍然假设位置原点为0）：
      \$\$
      f_{\text {ext }}=m_{d} \ddot{x}+b_{d} \dot{x}+k_{d} x
      \$\$

    + 通常，我们通过位置传感器获得方块当前的位置以及运动速度，并求出理想的加速度以实现设定的阻抗目标。我们有：
      \$\$
      \ddot{x}_{d}=\frac{1}{m_{d}}\left(b_{d} \dot{x}+k_{d} x-f_{\text {ext }}\right)
      \$\$

    + 求出理想的加速度后，我们就可以根据当前的位置和速度求出理想的位置/速度指令，使系统呈现出我们理想的阻抗。
      \$\$
      \begin{aligned}
      &\dot{x}_{d}=\dot{x}+\ddot{x}_{d} \cdot \delta t \\
      &x_{d}=x+\dot{x}_{d} \cdot \delta t
      \end{aligned}
      \$\$

    + 这个方法可以很好地利用现在已非常成熟的位置控制算法来实现精度和响应都比较好的导纳控制。另外，它也不需要依赖系统本身的动力学参数。但是可以看到，处在分母的m_d项使得我们很难让系统呈现比较小的质量/惯性——很小的扰动即可能造成很大的运动指令变化，系统也容易不稳定。

  + 对比总结：

    ![image-20210728222732738](https://tva1.sinaimg.cn/large/008i3skNgy1gsxokyqi1ij30mf09q3z8.jpg)


### Reference

- [1] [Robotics公众号](https://www.guyuehome.com/author/cc)：以上很多内容从中整理，我觉得写得非常好，可以很快梳理机器人学一些知识。
