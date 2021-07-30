---
layout:     post                    # 使用的布局（不需要改）
title:      前馈控制与MPC控制             # 标题 
subtitle:   Forward Control and MPC  #副标题
date:       2021-07-30              # 时间
author:     Henry                      # 作者
header-img: img/post-bg-os-metro    #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - Technology
---

### 前馈控制

前馈控制在机械臂中有比较多的使用，主要是在已知外力输入的情况下所做的控制，对于机械臂这种模型能建立，同时其易受到重力影响的系统而言，在控制上是不可或缺的一个部分。

如果我们只用PID反馈控制，相当于我们不需要任何动力学建模，那我们最终设定的PID参数并不能完美适合系统，相当于引入了很大的系统扰动，这样我们不能完美地控制机器人。既然我们靠反馈无法把扰动全压下去，那我们是否可以考虑，通过预测外界扰动，再通过前馈补偿来解决呢？这时就可以构建一个由PID反馈控制+基于系统建模的前馈控制的控制器。

![image-20210729214153874](https://tva1.sinaimg.cn/large/008i3skNgy1gsywh22rv8j31a00ekq4x.jpg)

在很多场合里面，机器人执行器所使用的控制方法一般为只是利用三环反馈控制，其控制效果在静态时候还是非常不错的，不过对于动态，比如实时的追踪一个轨迹还是不太可以的，一般也只是采用位置控制的方法：

![电机控制进阶3——PID串级控制（附全套代码下载） - 哔哩哔哩](data:image/png;base64,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)

对于机械臂控制而言，采用**前馈+三环控制**这种策略是在三环反馈控制的基础上，加入前馈指令值。驱动器一般需工作在CSP模式，前馈值的生成方式有两种：控制器还是只发送给机械臂指令位置，驱动器通过差分生成前馈速度值及前馈加速度值；控制器不止发送指令位置，它还将前馈速度偏置值、前馈力矩偏置值也发送给驱动器。

![image-20210729222404064](https://tva1.sinaimg.cn/large/008i3skNgy1gsywhpi9psj30su0cu0tl.jpg)

#### 举个例子

如下系统动力学建模为：
\$\$
\tau=M \theta^{\prime \prime}+m g r \cos \theta+b \theta^{\prime}\tau=M \theta^{\prime \prime}+m g r \cos \theta+b \theta^{\prime}
\$\$
![image-20210729223215646](https://tva1.sinaimg.cn/large/008i3skNgy1gsywhsoch0j30nw0fit94.jpg)

其中M是连杆绕转轴的转动惯量，m是连杆的质量，r是转轴到连杆质心的距离，g是重力加速度。实际情况中转轴处会存在摩擦力，这里简单假设摩擦力为粘滞阻力系数b。然后就是丢进去Matlab中进行仿真，假如只是使用到PD控制器（在很多机器人系统中为了不产生震荡大多数的控制器会采用PD控制，不过在Innfos中他的控制器设计为PD控制）

PID控制不考虑机器人的动力学特性，只按照偏差进行负反馈控制，即只有在得到误差信号后才能输出控制量。另一种控制策略是根据机器人动力学模型预先产生控制力/力矩。动力学模型为：

\$\tau=M \theta^{\prime \prime}+m g r \cos \theta+b \theta^{\prime} \tau=M \theta^{\prime \prime}+m g r \cos \theta+b \theta^{\prime}\$

假如建模建立和很不错的前提下，在生成末端轨迹之后，机器人关节每一个瞬间都可以得到他的角度，角速度，和角加速度，代入上述式子就可以获得其力矩大小，然后直接使用力矩前馈输入即可，不过虽然前馈在理论上很完美，但是在实际中却依然避免不了各种误差的存在，所以一般采用的是前馈与反馈并行控制的方法，Matlab仿真如下：

![image-20210729230258203](https://tva1.sinaimg.cn/large/008i3skNgy1gsywhybodsj31y40ogmza.jpg)

其中的子模块如下：

![image-20210729230346147](https://tva1.sinaimg.cn/large/008i3skNgy1gsywi3f7ivj31680b0q3l.jpg)

其最终的效果就可以去弥补重力问题，在对于很多被控对象如（云台类的）是很有效的。

### MPC（Model Predictive Control）

算法具有三要素：

+ 模型
+ 预测
+ 滚动优化
+ 误差补偿

与其他控制算法的对比：

+ **PID：**无需模型、但是不适合非线性；
+ **LQR：**对模型精度要求很高，在无限长的时间域内做优化控制；
+ **MPC：**需要模型，但是对于模型要求不高（其中具有误差补偿，能修正模型）

其主要的思想是：

+ 在\$a_1\$时刻对系统施加一个力，然后因为模型已经确定，计算下一个时刻（\$a_2\$）系统的状态，然后与实际的状态做对比，然后计算（\$a_3\$）时刻的状态，最后能得到其中的误差有多少和能能量有多少，去按此做最优化处理即可。

模型预测控制是一种超前控制策略, 对具有纯滞后环节的系统有显著的控制效果[[4](javascript:void(0);)]。通过局部最优解的动态计算, 以达到全局最优控制。根据系统结构建立预测控制模型, 运用模型预测控制策略解决大扰动大滞后系统动态特性差, 系统控制不稳定的问题。通过仿真实验和工程项目验证, 表明该策略适用于此类系统的控制。

模型预测控制是一种基于计算机控制的超前控制策略。该策略利用控制系统模型和当前过程的反馈测量值在满足约束条件下, 计算控制变量的未来动作。而后将计算好的控制量输出到调节控制回路中, 由调节器完成控制作用。控制策略基于系统的阶跃响应, 适用于稳定的线性控制系统, 对于具有纯滞后和非最小相位系统的控制有非常显著的优势。

+ 控制框图如下所示：

![image-20210730112003848](https://tva1.sinaimg.cn/large/008i3skNgy1gsywi7l1ttj30yf0awdh8.jpg)

+ 模型建立：

  模型建立一般会采用参数辨识的方法，一般做法是采用对系统施加一个阶跃响应，然后将系统响应给Matlab中的参数辨识工具箱，在做参数辨识的时候要最好数学建模一下去做拟合，这样可以比较快速的获得比较具有现实意义的辨识结果，不过要是没有建模直接采用工具箱中设定一定的阶数也可以的（一般为2-4阶左右，有些系统会到6阶，不过要注意overfitting问题）

  ![image-20210730115332773](https://tva1.sinaimg.cn/large/008i3skNgy1gsywiav4lpj30yx09375b.jpg)

+ 系统为离散系统，根据系统的开环传递函数, 用离散化后的传递函数来描述系统的动态特性。动态系数为\$a_{1}, a_{2}, \ldots a_{p}\$为单位阶跃响应在各个采样点的值。

  \$p\$为模型时域长度;\$a_p\$为系统稳态值的系数;\$T\$为系统采样周期。单位阶跃响应曲线如下图所示。

  ![image-20210730115611034](https://tva1.sinaimg.cn/large/008i3skNgy1gsywiem4dsj30mj0fj3z5.jpg)

+ 系统的输出效果由叠加原理可得，其思想和卷积一样：
  \$\$
  y(k)=\sum_{i=1}^{p-1} a_{i} \Delta u(k-i)+a_{p} \Delta u(k-p)(j=1,2, \cdots, n)
  \$\$

+ 这样就可以从输出去预测下一步的状态：
  \$\$
  \hat{y}(k+j)=\sum_{i=1}^{p-1} a_{i} \Delta u(k+j-i)+a_{p} \Delta u(k+j-p)(j=1,2, \cdots, n)
  \$\$

+ 这样就可以求出未来n步的输出结果：

  状态变量：
  \$\$
  \begin{aligned}
  &\left[\begin{array}{c}
  \hat{y}(k+1) \\
  \hat{y}(k+2) \\
  \vdots \\
  \hat{y}(k+n)
  \end{array}\right]=\left[\begin{array}{cccc}
  a_{1} & & & 0 \\
  a_{2} & a_{1} & & \\
  \vdots & & \ddots & \\
  a_{n} & a_{n-1} & \cdots & a_{1}
  \end{array}\right]\left[\begin{array}{c}
  \Delta u(k) \\
  \Delta u(k+2) \\
  \vdots \\
  \Delta u(k+m-1)
  \end{array}\right]+\\
  &\left[\begin{array}{c}
  y_{0}(k+1) \\
  y_{0}(k+n) \\
  \vdots \\
  y_{0}(k+n)
  \end{array}\right]\\
  \end{aligned}
  \$\$
  输出观测变量：
  \$\$
  \widehat{\boldsymbol{Y}}=[\hat{y}(k+1), \hat{y}(k+2), \cdots, \hat{y}(k+n)]^{\mathrm{T}}
  \$\$

+ 变量矩阵写成A（和系统自身建模参数有关）：
  \$\$
  \boldsymbol{A}=\left[\begin{array}{cccc}
  a_{1} & & & 0 \\
  a_{2} & a_{1} & & \\
  \vdots & & \ddots & \\
  a_{n} & a_{n-1} & \cdots & a_{1}
  \end{array}\right]
  \$\$

+ 最后能直接写成：
  \$\$
  \widehat{\boldsymbol{Y}}=\boldsymbol{A} \Delta \boldsymbol{U}+\boldsymbol{Y}_{0}
  \$\$
  其中矩阵\$A\$为\$n×m\$维常数矩阵,\$ n\$和\$m\$​分别为最大预测长度和控制长度。

+ 滚动优化：获得当前状态，计算输入，计算输入后状态状态，计算输入，计算输入后状态状态......一直持续下去，直到达到预计预测的次数。

  ![image-20210730120855859](https://tva1.sinaimg.cn/large/008i3skNgy1gsywiiungsj30mf0kudgk.jpg)

+ 目标函数计算：

  模型预测控制在滚动优化的过程中, 需要确定一个目标函数保证每次优化都会取得一个最优解, 保证控制系统性能指标达到最优。应力控制系统目标函数取控制对象在未来的采样点上跟踪一条期望的参考轨迹, 要求参考轨迹和未来采样点上的输出值方差最小, 则模型的目标函数为：
  \$\$
  J=\sum_{j=1}^{n}[y(k+j)-w(k+j)]^{2}+\sum_{j=1}^{m} \lambda(j)[\triangle u(k+j-1)]^{2}
  \$\$
  他的目的就是在未来几步中会和目标值尽可能接近（方差最小），当*J*值取得最小时, 系统在未来*n* (*p*<*n*<*m*) 个时刻的输出值会尽可能接近期望值。为了计算简便, 取控制加权的系数*λ* (*j*) =*λ*, *λ*取常数。令:
  \$\$
  W=[w(k+1), w(k+2), \ldots, w(k+n)]^{T}
  \$\$
  则有:
  \$\$
  \mathrm{J}=(\mathrm{Y}-\mathrm{W})^{T}(\mathrm{Y}-\mathrm{W})+\lambda \Delta \mathrm{U}^{T} \Delta \mathrm{U}
  \$\$

+ #### 参考轨迹

  为避免控制系统出现输入和输出剧烈变化, 系统的输出值必须沿着一条期望的参考轨迹曲线达到设定值。参考轨迹采用指数变化的形式，一般是采用一个惯性环节去做轨迹，然后控制其中的\$\alpha\$大小即可，其实就是对应了一阶环节中的时间常数大小:
  \$\$
  w(k+i)=a^{i} y(k)+\left(1-\alpha^{i}\right) y_{r}
  \$\$

+ 反馈矫正

  开环控制不能稳定的跟踪参考轨迹，所以需要引入反馈矫正的方法减少误差，直接读公式即可：
  \$\$
  e(k+1)=y(k+1)-\widehat{\boldsymbol{y}}(k+1)
  \$\$
  矫正后的输出变量：
  \$\$
  \boldsymbol{y}_{\text {cor }}(k+1)=\widehat{\boldsymbol{y}}_{p}(k+1)+\boldsymbol{H e}(k+1)
  \$\$
  其中的H为误差矫正矩阵：
  \$\$
  \boldsymbol{H}=\left[\begin{array}{c}
  h(1) \\
  \vdots \\
  h(p)
  \end{array}\right]
  \$\$
  在*k*+1时刻, 由于时间基点的变动, 预测的未来时间点移到\$k+2, …, k+p+1\$。初始预测值也相应移位, 其初始预测值\$\hat{y}_{0}(k+1)\$，移位后的结果为：
  \$\$
  \hat{y}_{0}(k+1)=S \hat{y}_{\text {cor }}(k+1)
  \$\$
  其中移位矩阵S为：
  \$\$
  \boldsymbol{S}=\left[\begin{array}{cccc}
  0 & 1 & \cdots & 0 \\
  0 & \ddots & \ddots & \vdots \\
  \vdots & \ddots & 0 & 1 \\
  0 & \cdots & 0 & 1
  \end{array}\right]
  \$\$

+ MPC控制总结为：
  + 模型建立（实际使用阶跃响应+参数辨识）；
  + 预测（输入+模型=预测）；
  + 滚动优化（可以以方差最小/能量最小去做优化，优化方法在Matlab中quadprog或是DU = (A'*Q*A+R)^-1*A'*Q*(W-Y0);u(k) = u(k-1) + DU(1,1);）；
  + 误差补偿；

### Reference

- [1] [Robotics公众号](https://www.guyuehome.com/author/cc)：以上很多内容从中整理，我觉得写得非常好，可以很快梳理机器人学一些知识。
