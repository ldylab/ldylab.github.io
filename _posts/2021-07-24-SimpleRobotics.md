---
layout:     post                    # 使用的布局（不需要改）
title:      二自由度机器人正逆解算手动实现              # 标题 
subtitle:   Robotics  #副标题
date:       2021-07-24              # 时间
author:     Henry                      # 作者
header-img: img/home-bg-art.jpg    #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - Technology
---
> 在机器人建模中一般会使用到很多的经典的建模工具，比如ROS中直接使用Gazebo+MoveIt工具箱，Matlab中使用Robotics ToolBox等，不过自己实践建模，手动实现机器人的正逆解算对于基本功而言是非常有益的，这里主要是参考了Github项目与知乎相关文章实现对于两个自由度“机械臂”正逆解算与可视化。

对于简单的关节机器人而言，其实其正逆解算只要采用非常直观直觉的方式，就可以实现其正逆解算的过程。

### 模型建立

![image-20210727145900489](https://tva1.sinaimg.cn/large/008i3skNgy1gsvinz8ao6j30ku0fqaai.jpg)

其中：

+ shoulder——肩部；Elbow——肘部；Wrist——腕部；

+ 手腕Wrist本身不是一个关节，但是我们可以把它视为我们的末端执行器（end-effector）。

+ 将shoulder肩膀约束到原点，根据**Forward Kinematics**，我们可以编写肘部和腕部的运动学方程

+ 其建模过程非常非常直观：

  - ![[公式]](https://www.zhihu.com/equation?tex=elbow_x+%3D+l_0cos%28%5Ctheta_0%29)
  - ![[公式]](https://www.zhihu.com/equation?tex=elbow_y+%3D+l_osin%28%5Ctheta_0%29)
  - ![[公式]](https://www.zhihu.com/equation?tex=wrist_x+%3D+elbow_x+%2B+l_1cos%28%5Ctheta_0%2B%5Ctheta_1%29%3Dl_0cos%28%5Ctheta_0%29%2B+l_1cos%28%5Ctheta_0%2B%5Ctheta_1%29)
  - ![[公式]](https://www.zhihu.com/equation?tex=wrist_y+%3D+elbow_y+%2B+l_1sin%28%5Ctheta_0%2B%5Ctheta_1%29%3Dl_0sin%28%5Ctheta_0%29%2B+l_1sin%28%5Ctheta_0%2B%5Ctheta_1%29)

+ 由于手腕是我们的末端执行器，因此我们将其坐标称为x和y。 这样我们末端执行器的**正向运动学**就可以得到：

+ - ![[公式]](https://www.zhihu.com/equation?tex=x%3Dl_0cos%28%5Ctheta_0%29%2Bl_1cos%28%5Ctheta_0%2B%5Ctheta_1%29)
  - ![[公式]](https://www.zhihu.com/equation?tex=y+%3D+l_0sin%28%5Ctheta_0%29%2Bl_1sin%28%5Ctheta_0%2B%5Ctheta_1%29)

### 模型求解

#### theta0 求解

![image-20210727154356121](https://tva1.sinaimg.cn/large/008i3skNgy1gsvk8zq5scj30ku0e0t99.jpg)

- 从末端执行器到机械臂底座的距离，我们记为r ， r 与 x, y 有如下关系：
- ![[公式]](https://www.zhihu.com/equation?tex=r%5E2+%3D+x%5E2%2By%5E2)
- 接着，根据余弦定理， r的平方又可以写成：
- ![[公式]](https://www.zhihu.com/equation?tex=r%5E2+%3D+%7Bl_0%7D%5E2%2B%7Bl_1%7D%5E2-2l_0l_1cos%28%5Calpha%29%3D+%7Bl_0%7D%5E2%2B%7Bl_1%7D%5E2-2l_0l_1cos%28%5Cpi-%5Ctheta_1%29)
- 根据该式，有：
- ![[公式]](https://www.zhihu.com/equation?tex=%5Ctheta_1+%3D+cos%5E-1%28%5Cfrac%7Bx%5E2%2By%5E2-%7Bl_0%7D%5E2-l_1%5E2%7D%7B2l_0l_1%7D%29)

#### theta1求解

![image-20210727154716063](https://tva1.sinaimg.cn/large/008i3skNgy1gsvk806enzj60ku0e0js102.jpg)

根据该几何关系，与theta0求解类似，容易得到：
- ![[公式]](https://www.zhihu.com/equation?tex=%5Ctheta_0+%3D+%5Ctan%5E%7B-1%7D%28%5Cfrac%7By%7D%7Bx%7D%29-%5Ctan%5E%7B-1%7D%28%5Cfrac%7Bl_1%5Csin%28%5Ctheta_1%29%7D%7Bl_0%2Bl_1%5Ccos%28%5Ctheta_1%29%7D%29)

程序实现（主要是参考了PythonRobotics）

```python
"""1.Robot Arm简单可视化"""
from math import cos,sin
import numpy as np
import matplotlib.pyplot as plt

class TwoLinkArm:
    """
    2关节机械臂可视化
    """
    def __init__(self,joint_angles=[0,0]):
        self.shoulder = np.array([0,0]) # 肩膀为0点
        self.link_lengths = [1,1] # 两臂长度均为1,1
        self.update_joints(joint_angles) # 更新joint位置

    def update_joints(self,joint_angles):
        self.joint_angles = joint_angles
        self.forward_kinematics()

    def forward_kinematics(self):
        """顺向运动学，给入两个角度进行计算"""
        theta0 = self.joint_angles[0]
        theta1 = self.joint_angles[1]
        l0 = self.link_lengths[0]
        l1 = self.link_lengths[1]
        self.elbow = self.shoulder+np.array([l0*cos(theta0),l0*sin(theta0)]) # 肘的位置更新
        self.wrist = self.elbow+np.array([l1*cos(theta0+theta1),l1*sin(theta0+theta1)]) # 腕部位置更新

    def plot(self):
        plt.plot([self.shoulder[0], self.elbow[0]],
                 [self.shoulder[1], self.elbow[1]],
                 'r-') # 肩部到肘部的Link
        plt.plot([self.elbow[0], self.wrist[0]],
                 [self.elbow[1], self.wrist[1]],
                 'r-') # 肘部到腕部的Link
        plt.plot(self.shoulder[0], self.shoulder[1], 'ko') # 肩部Origin
        plt.plot(self.elbow[0], self.elbow[1], 'ko') # 肘 Joint
        plt.plot(self.wrist[0], self.wrist[1], 'ko') # 腕 Joint

# 仿真参数
Kp = 20 # 到达目标点的速度控制
dt = 0.01

# Link Lengths
l1=l2=1

# 将初始目标位置设置为初始末端执行器位置
x = 2
y = 0

show_animation = True

if show_animation:
    plt.ion() #  动态视图

def two_joint_arm(GOAL_TH=0.0,theta1=0.0,theta2=0.0):
    """
    计算平面2DOF臂的逆运动学
    GOAL_TH: 距离目标的判别阈值
    """
    while True:
        try:
            """逆运动学"""
            theta2_goal = np.arccos((x**2 + y**2 - l1**2 - l2**2) / (2 * l1 * l2))
            theta1_goal = np.math.atan2(y, x) - np.math.atan2(l2 *\
                          np.sin(theta2_goal), (l1 + l2 * np.cos(theta2_goal)))
            if theta1_goal < 0:
                theta2_goal = -theta2_goal
                theta1_goal = np.math.atan2(y, x) - np.math.atan2(l2 *
                          np.sin(theta2_goal), (l1 + l2 * np.cos(theta2_goal)))

            # 更新theta1,theta2
            theta1 = theta1 + Kp * ang_diff(theta1_goal, theta1) * dt
            theta2 = theta2 + Kp * ang_diff(theta2_goal, theta2) * dt
        except ValueError as e:
            print("无法到达的位置")

        wrist = plot_arm(theta1,theta2,x,y)

        # 检查goal
        d2goal = np.hypot(wrist[0] - x, wrist[1] - y)

        if abs(d2goal) < GOAL_TH and x is not None:
            return theta1, theta2

def ang_diff(theta1, theta2):
    # 返回两个角度之差，范围在[-pi,pi]
    return (theta1 - theta2 + np.pi) % (2 * np.pi) - np.pi

def plot_arm(theta1, theta2, x, y):
    """改版，动态可视化"""
    shoulder = np.array([0, 0])
    elbow = shoulder + np.array([l1 * np.cos(theta1), l1 * np.sin(theta1)])
    wrist = elbow + \
        np.array([l2 * np.cos(theta1 + theta2), l2 * np.sin(theta1 + theta2)])

    if show_animation:
        plt.cla()
        plt.plot([shoulder[0], elbow[0]], [shoulder[1], elbow[1]], 'k-')
        plt.plot([elbow[0], wrist[0]], [elbow[1], wrist[1]], 'k-')
        plt.plot(shoulder[0], shoulder[1], 'ro')
        plt.plot(elbow[0], elbow[1], 'ro')
        plt.plot(wrist[0], wrist[1], 'ro')
        plt.plot([wrist[0], x], [wrist[1], y], 'g--')
        plt.plot(x, y, 'g*')
        plt.xlim(-2, 2)
        plt.ylim(-2, 2)
        plt.show()
        plt.pause(dt)

    return wrist

def click(event):
    # 鼠标点击触发事件，作为导航点
    global x, y
    x = event.xdata
    y = event.ydata

def animation():
    from random import random
    global x, y
    theta1 = theta2 = 0.0
    for i in range(5):
        x = 2.0 * random() - 1.0 # 末端随机位置
        y = 2.0 * random() - 1.0
        theta1, theta2 = two_joint_arm(
            GOAL_TH=0.01, theta1=theta1, theta2=theta2)

def main():  # pragma: no cover
    fig = plt.figure()
    fig.canvas.mpl_connect("button_press_event", click)
    # 使用esc键停止模拟。
    fig.canvas.mpl_connect('key_release_event',
            lambda event: [exit(0) if event.key == 'escape' else None])
    two_joint_arm()

if __name__== '__main__':
    main()
```

### Reference

- [1] [PythonRobotics](https://github.com/AtsushiSakai/PythonRobotics)

- [2] [知乎专栏](https://zhuanlan.zhihu.com/p/106854472)
