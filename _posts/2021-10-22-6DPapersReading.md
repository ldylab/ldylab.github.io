---
layout:     post                    # 使用的布局（不需要改）
title:      6D位姿估计跟踪论文阅读            # 标题 
subtitle:   6D Papers Reading  #副标题
date:       2021-10-22              # 时间
author:     Henry                      # 作者
header-img: img/home-bg-M1.jpeg   #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - PaperReading
---



## 6D位姿估计跟踪

### Paper1：BundleTrack：6D Pose Tracking for Novel Objects without Instance or Category-Level 3D Models

> 论文地址：https://arxiv.org/abs/2108.00516
>
> 论文代码：https://github.com/wenbowen123/BundleTrack

#### 简介

大多数先前的6D物体姿态跟踪通常假定目标物体的CAD模型，至少是需要一个类别级别的CAD模型，其模型可用于离线训练或在线模板匹配期间。而本文作者提出了BundleTrack，一个用于新型物体的6D姿势跟踪的通用框架，它不依赖于3D模型，无论是在实例还是类别层面。它利用了深度学习的最新进展的互补属性，用于分割和稳定的特征提取，利用记忆特性的去优化保证位姿图像时空的一致性（as well as memory-augmented pose graph optimization for spatiotemporal consistency.)。这使得在各种比较复杂的情况下，包括明显的遮挡和物体运动，都能实现长期的、低漂移的跟踪。两个公共基准的综合实验表明，所提出的方法明显优于最先进的类别级6D跟踪或动态SLAM方法。与依靠物体实例CAD模型的先进方法相比，尽管所提出的方法减少了信息要求，但仍取得了相当的性能。在CUDA中的有效实现为整个框架提供了10Hz的实时性能。

#### 问题提出

以前在该领域内的研究：

+ 在之前的研究中都是以假定可以获得一个物体实例的三维模型，但是这种实例的三维模型使得对新的，未见过的实例变得很复杂（在训练过程中都是采用有CAD模型的实例，在面对没有CAD模型的情况下效果并不好）。
+ 在之前的研究中为了克服需要精确的CAD模型，部分研究（如上述Paper2）提出了采用类别级别的三维模型来进行6D位姿估计，其在训练的过程中依然需要大量的CAD模型，目前主流的数据库并不能涵盖现实世界中的各类物品，其仍具有局限性，并且虽然在Paper2提出了采用虚拟混合显示的方法，但是其还是需要不小的手工操作。
+ 在SLAM领域中，也有部分的工作与此类似，即在跟踪动态对象的同时对其进行重建，而不需要事先建立对象的三维模型。不过，通过重建进行跟踪，在将观测值与错误的姿态估计融合到全局模型中是，往往会积累错误。

作者问题提出：

+ 在上述限制的激励下，这项工作旨在实现准确、稳健的6D姿态跟踪，可用于没有实例或类别级3D模型的新物体。
  + 它利用了视频分割的最新进展以及基于学习的关键点检测和匹配，来进行粗略的姿势估计；
  + 通过记忆增强的姿势图优化步骤来实现时空一致的姿势输出；
  + 代表性的历史观测数据不是汇总到一个全局模型中，而是作为关键帧保存在一个存储池，为未来的图形提供候选节点，以便与最新的观测数据一起实现多对数据关联；
  + CUDA并行计算。

#### 创新点

+ 对各种方法进行了新的整合，形成了一个6D姿态跟踪框架，该框架可以通用于新的物体，而无需访问实例或类别级的3D模型。
+ 为低漂移准确的6D物体姿势跟踪提供了一个记忆增强的姿势图优化。特别是，用历史观测数据增强内存池，可以实现多跳数据关联，改善一对连续帧之间对应关系的不足。此外，将关键帧保持为原始节点，而不是汇总到一个全局模型中，这大大减少了跟踪漂移。
+ 一个有效的CUDA实现，允许在线执行计算量大的多对特征匹配以及6D物体姿势跟踪的姿势图优化（就作者所知，这是第一次）。

#### 核心思想

问题简述：

假设一个刚体没有对应的三维模型，也没有其类别级的三维模型数据库，目标连续跟踪其从一开始时的，6D位姿变换，即相对变换：

$$
T_{0 \rightarrow \tau} \in S E(3), \tau \in\{1,2, \ldots, t\}
$$

也就是说在视频中的每一帧，都能确定图像中物体的位姿。

整个系统的输入如下：

+ $I_{\tau}$：一个RGB-D数据系列：$I_{\tau}, \tau \in\{0, \ldots, t\}$​

+ $M_{0}$：第一幅图像上的二进制掩码$I_0$​​，表示要追踪图像空间中的目标对象区域。

  > **Notes：**
  > 初始掩码$M_0$​可以通过多种不同的方式获得，以初始化跟踪。例如，通过语义分割或非语义方法，如图像分割，点云分割/聚类，或平面切除，等等。

+ $T_{0}^{C}$：相机帧𝐶中的初始姿态。 如果目标是恢复物体在𝐶中的绝对姿态，则使用该参数，否则不需要设置。

+ ![image-20211024102648975](https://tva1.sinaimg.cn/large/008i3skNgy1gvrbzhg0a4j60r10c675502.jpg)

+ 物体在摄像机框架𝐶中的姿势可以在任何时间戳通过应用相对变换𝑇 0→𝜏在摄像机框架中恢复，为简单起见，本文其余部分将把T 𝜏称为该过程的输出，但𝑇 0→𝜏是实际计算的跟踪。

  $$
  \mathbf{T}_{\tau}=T_{\tau}^{C}=T_{0}^{C}\left[\left(T_{0}^{C}\right)^{-1} T_{0 \rightarrow \tau} T_{0}^{C}\right]=T_{0 \rightarrow \tau} T_{0}^{C} \in S E(3)
  $$

#### 模型

![image-20211024112722914](https://tva1.sinaimg.cn/large/008i3skNgy1gvrbzml4hej61jg0u0n8z02.jpg)

> **Notes：**
>
> 翻译：图像配准（Image Registration）

模型算法细节分析：

+ ![image-20211024124033692](https://tva1.sinaimg.cn/large/008i3skNgy1gvrbzpnt23j61yh0u07hl02.jpg)
+ ![image-20211024124046166](https://tva1.sinaimg.cn/large/008i3skNgy1gvrbztum89j61rk0u0guv02.jpg)
+ ![image-20211024124058604](https://tva1.sinaimg.cn/large/008i3skNgy1gvrbzxlhu5j62030u0qd802.jpg)
+ ![image-20211024124117553](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc00vscej61t60u0guu02.jpg)
+ ![image-20211024124129678](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc0650ahj623k0tqwnt02.jpg)

#### 实验结果

##### 数据集

+ 数据集一：文章中使用的是NOCS数据集，该数据集中是包含了6个物体类别：瓶子、碗、相机、罐子、笔记本电脑和杯子。训练集由以下部分组成：
  + 7个真实视频，每个类别共包含3个实例，并有地面真实姿势的注释；
  + 275K帧的合成数据，使用上述6个类别的1085个实例，使用三维模型数据库ShapeNetCore，在每个场景中使用随机姿势和物体组合生成。测试集有6个真实的视频，在每个类别中包含3个不同的未见过的实例，导生成18个不同的物体实例和3200个框架。
  + 这个数据集有助于验证机器人操作过程中6D姿势跟踪的有效性，它最初是为了评估依赖于CAD模型的方法而开发的，不过现有的CAD模型并没有被BundleTrack使用。
+ 数据集二：文章中 使用的第二个数据集是YCBInEOAT数据集，包含9个视频序列，由静态的RGB-D摄像机拍摄，同时物体被动态地操纵。有三种类型的操纵：1）单臂取放，2）手内操纵，3）臂间取放。这些情况和所使用的末端检测器使得直接从前向运动学计算姿势不可靠。操纵视频涉及5个YCB对象：芥末酱瓶、番茄汤罐、糖盒、漂白剂清洁剂和饼干盒。

##### 测试结果

+ **NOCS数据集测试结果：**

  ![image-20211024150222772](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc0cne5hj61i30u0wpg02.jpg)

  ![image-20211024150522319](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc0k278jj60ih0puq8p02.jpg)

  如表一所示，尽管没有访问实例或类别级别的三维模型，BundleTrack在所有指标和所有物体类别上都明显优于对比点。

+ **YCBInEOAT Dataset：**

  只对移动相机拍摄的静态物体进行评估，不能完全反映6D姿态跟踪方法的特性。出于这个原因，文章选择了YCBInEOAT数据集来评估物体在相机前移动的情况下的跟踪。通过和NOCS数据集相同的评估协议。结果如下图所示：

  ![image-20211024150858586](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc0no550j61dz09udjk02.jpg)

  分析：对于6-PACK来说，每一帧都提供由前向运动学计算的物体模型的三维边界框，以从点云中裁剪出ROI，因为它比其默认的通过估计运动推断三维边界框的模块更可靠。对于MaskFusion和BundleTrack，最初的物体Masks是自己定义的，然后是在PCL中实现的欧几里得聚类法。由于缺乏训练集，原MaskFusion的分割模块Mask-RCNN不能在这个基准上重新训练。因此，在跟踪过程中，目标对象的掩码是通过从前向运动学中分割出机器人手臂和末端eﬀector的区域来计算的。对于6PACK所训练的 "瓶子 "类别中的不规则形状或颜色的实例（"021_漂白剂_清洁剂"、"006_芥末瓶"），它很难获得令人满意的结果。同时BundleTrack始终表现出高质量的跟踪，而不需要任何重新训练或微调。这证明了BundleTrack对新的物体实例的通用性，无论它们在该类别中的分布属性如何。即使与依靠物体实例CAD模型的方法相比，BundleTrack也取得了相当或优越的性能。

+ 测试结果分析：

  性能分析：

  ![image-20211024151353799](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc0x1d2dj60k80ontc102.jpg)

  在文章中主要是利用了4大指标来分析系统：

  + Ablations Study（图a）：主要是研究在线全局姿势图优化和每个衡量指标的有效性；
  + 对于初始位置的灵敏度（图b）：4厘米范围内的随机平移噪声被添加到初始姿势中。这一部分进一步研究了不同的平移和旋转噪声水平下的鲁棒性；
  + 计算时间（图c）：各模块的平均运行时间。整个框架的平均运行时间为10Hz，包括视频分割。相关工作中的6-PACK、TEASER++和MaskFusion方法在同一台机器上分别以4Hz、11Hz和17Hz运行；
  + 追踪漂移分析（图d）：与以前代表性方法的相比，旋转和平移误差的时间。

  总结：

  在BundleTrack中，神经网络的权重和超参数是固定的，不需要在所有评估中进行任何重新训练或微调。当应用于新的实例时，该框架不需要访问实例或类别级三维模型进行训练。

#### 不足

虽然BundleTrack在所有实验中都能稳健地保持跟踪，没有丢失或重新初始化，但也观察到中间的不精确估计情况，如下图所示的情况：

![image-20211024152047393](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc11bc1cj60o60j4wj802.jpg)

#### 个人测试与遇到的相关问题

我个人也是在最近开始尝试跑通作者提供的代码，不过在按照作者提供的Docker配置环境后，按照其步骤一步一步来，在跑NOCS数据集的训练过程中一直出现"Segmentation fault"问题。

![image-20211024152644542](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc15ht45j60oq0l376o02.jpg)

![image-20211024152652557](https://tva1.sinaimg.cn/large/008i3skNgy1gvrcirkiy8j61280dqgrg02.jpg)

在运行YCBInEOAT数据集时，发现作者文件中少了.obj文件，自己在添加之后也还是出现了如下问题：

![image-20211024152814393](https://tva1.sinaimg.cn/large/008i3skNgy1gvrciuqlqfj611q0f241702.jpg)

在将cloud的size为0的修改暂时不写入，后能部分运行程序：

![image-20211024152943373](https://tva1.sinaimg.cn/large/008i3skNgy1gvrcixo6bwj61400p0th002.jpg)

目前遇到的问题可能是由于作者提供的数据缺失部分文件，已经邮件咨询并在github中issue中反馈，并也看到了有人在issue反馈中提到了和我一样遇到的问题，不过目前作者并还有对此问题进行回复。





### Paper2: 6-PACK: Category-level 6D Pose Tracker with Anchor-Based Keypoints

> 论文地址：https://arxiv.org/abs/1910.10750v1
>
> 论文代码：https://github.com/j96w/6-PACK

#### 简介

作者提出了一个基于RGB-D的深度学习方法6PACK，能够实时的跟踪已知类别物体。通过学习用少量的三维关键点来简洁地表示一个物体，基于这些关键点，通过更加点匹配来估计物体在帧与帧之间的运动。这些关键点使用无监督端到端学习来实现有效的跟踪。其实验结果表明支持机器人执行简单的基于视觉的闭环操作任务。

#### 问题提出

在机器人抓取任务中，实时跟踪物体6D位姿的能力影响抓取任务的实施。现有的6D跟踪方法大部分是基于物体的三维模型进行的，有较高的准确性和鲁棒性。然而在现实环境中，很难获得物体的三维模型，所以作者提出开发一种类别级模型，能够跟踪特定类别未见过的物体。

#### 创新点

+ 这种方法不需要已知道物体的三维模型。论文中采用的是Anchor机制，类似于2D对象检测中使用的Proposal方法，来避免定义和估计绝对6D位姿。

  > **Notes：**
  >
  > 什么是Anchor机制？
  >
  > 什么是Proposal方法？

+ 这些Anchor为生成三维关键点提供了基础。与以往需要手动标注关键点的方法不同，提出了一种无监督学习方法，该方法可以最优的三维关键点集进行跟踪。
+ 这些关键点能够简洁的表达物体，可以有效地估计相邻两帧之间位姿的差异。这种基于关键点的表示方法可以实现鲁棒的实时6D姿态跟踪。

#### 核心思想

作者提出的模型使用RGB-D图像，基于之前位姿周围采样的anchors（红点），来鲁棒地检测和跟踪一组基于3D类别的关键点（黄色）。然后利用连续两帧中预测的关键点，通过最小二乘优化求解点集对齐的问题，计算出6D物体的位姿变化。

![image-20211023160836985](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc1zafkhj615r0u0dn802.jpg)

#### 问题的定义

将类别级物体6D位姿跟踪定义为：物体在连续时间$t-1$和$t$之间的位姿变化问题。初始位姿是针对相同类别的所有目标物体定义的标准框架相对于相机框架的平移和选择。比如对于相机类别，其根据其大致的形状去确定其XYZ的位姿，将框架放置在物体的质心处，x轴指向相机物镜方向，y轴指向上方。

将3D关键点定义为：在整个时间序列中几何和语义上一致的点。给定两个连续的输入帧，需要从两帧中预测匹配关键点列表。基于刚体假设的基础，利用最小二乘优化来解决点集对齐问题，从而得到位姿变化的.

#### 模型

> **Notes：**
>
> 什么是DenseFusion？
>
> 什么是注意力机制（Attention）？

![image-20211023171150472](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc23anzlj61kk0u0aiu02.jpg)

6-PACK算法在预测位姿周围生成anchor网格的过程中使用了注意力机制。每个点用RGB-D点单独特征的距离加权和来表示体积。使用anchor信息在新的RGB-D框架中找到物体的粗略质心，并指导对其周围关键点的后续搜索，这比在无约束的三维空间中搜索关键点效率更高。

#### 实验结果

![image-20211023171754689](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc26zbu7j60u00z7q9602.jpg)

结果：

+ 6-PACK指标5°5cm比NOCS高出15%以上，指标IoU25高出12%。说明与使用所有输入像素作为关键点的NOCS相比，**6-PACK能够检测出最适合类别级6D跟踪的3D关键点**。实验结果如下图所示：

  ![image-20211023171907627](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc2btxnfj61fk0u0dtd02.jpg)

  其中，前两列为NOCS和6-PACK的定性对比，后两列为关键点匹配的结果。

+ 6-PACK所有指标都优于KeypointNet，KeypointNet经常跟丢。作者的方法避免了丢失物体的轨迹（IoU25>94%），**基于anchor的注意力机制提高了整体的跟踪性能**。

+ 为了检验不同方法的鲁棒性和稳定性，作者计算了没有前x帧的平均性能。这样就能测量出初始位姿对性能的影响（接近初始位姿的帧很容易跟踪）。如下图，除了NOCS之外，所有方法的性能都有所下降，因为NOCS是位姿估计方法，而不是位姿跟踪方法。在整个过程中，6-PACK的性能比NOCS高出10%以上，并在初始帧100后停止下降。

  ![image-20211023173333592](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc2fyvpqj61c40u044702.jpg)



### Paper3：Normalized Object Coordinate Space for Category-Level 6D Object Pose and Size Estimation

> 论文地址：https://arxiv.org/pdf/1901.02970.pdf
>
> 论文代码：https://github.com/hughw19/NOCS_CVPR2019

#### 简介

文章的目的是估计RGB-D图像中从未见过的物体的尺寸和在空间中的位姿。与以前方法中SSD-6D、latent霍夫投票、BB8、YOLO-6D和poseCNN都预先提供了物体精确的CAD模型及大小，不同的是，作者假设在训练或测试期间没有精确的CAD模型可以使用，这也是更加符合实际，在现实中大多数物体是没有CAD模型的。为了处理给定类别中不同的和从未见过的物体实例，作者引入了标准化坐标空间（NOCS），即是同一个类别中的所有物体（照相机是一个类的总称，照相机下面又再分了不同的品牌类型的相机）使用一个共享的标准模型来表示。然后，通过训练神经网络来推断观察到的像素与共享标准模型的对应关系以及其他信息，例如类别标签和mask。通过将预测图像与深度图相结合，共同估计杂乱环境下多个物体的6D位姿和尺寸。为了训练网络，作者提出了一种新的上下文感知技术来生成大量带注释的混合现实数据。为了进一步改进模型并评估他在真实数据上的性能，作者还提供了一个完全标注的真实场景下的数据集。大量实验表明，该方法对于真实场景中未见到过的物体能具有一定鲁棒性的估计其位姿与大小。

#### 问题提出

+ 位姿估计的现有方法中SSD-6D、Latent霍夫投票、BB8、YOLO-6D和possCNN都预先提供了物体的精确CAD模型及大小，然而从未见过的物体是没有CAD模型的。

+ 在三维目标检测的论文中不需要物体的CAD模型就可以估计类别标签和边界框，但是三维目标检测是根据视点估计的，没有编码物体的精确方向。

  上述这两种方法都不能对从未见过的物体进行位姿估计。

+ 难点：

  + 找到特定类别物体位姿和尺寸的表示方法；
  + 缺少训练和测试数据集；

#### 创新点

+ 作者使用一个共享的标准坐标空间（NOCS）作为参考系来表示同一类别中的所有物体的实例。
+ 提出一个可以同时预测物体类别标签（就是所识别物体是什么）、mask（把物体给标注出来）和NOCS（标准化空间）图的CNN，将NOCS图与深度图进行对应来估计从未见到过的物体的位姿和大小。
+ 使用空间上下文感知混合现实方法来自动生成大量数据用来训练和测试。

#### 核心思想

作者定义了一个共享的标准坐标空间，可以为从未见过的物体定义位姿和尺寸。标准物体坐标空间是包含在单位立方体中的三维空间。对于给定的物体类别，作者将物体实例放到NOCS中标准化（相当于所有相机都使用同一个模型，将中心和方向对齐后进行缩放来表示）。作者通过NOCS在RGB图像上的投影训练作者的网络。在测试时，网络回归NOCS图，然后将其与深度图一起用于6D位姿和尺寸估计。

![image-20211024015025704](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc2mdf5rj61220jcdik02.jpg)

#### 数据集

因为现有的三维目标检测数据集NYU v2 和SUN RGB-D只关注三维边界框，无法获得6D位姿和大小的ground truth。作者提出空间上下文感知的混合现实方法来自动生成大量数据用来训练和测试。选取31个真实的室内场景共553张图像，从ShapeNetCore数据集中挑选了6种物体：瓶子、碗、相机、罐子、笔记本电脑和杯子。**作者还创建了一个干扰物类别，由上面没有列出的类别中的物体类别组成，如显视器、电话和吉他。使场景中存在其他物体，这也提高了对主要类别进行预测的鲁棒性。**

**上下文感知合成**:为了提高真实感，作者以上下文感知的方式合成虚拟物体，将物体放在支撑平面上，使用合理的光照。使用平面检测算法对真实图像进行像素级平面分割。随后，在分割平面上随机采样位置和方向。然后放置几个虚拟光源来模拟真实的室内照明条件。最后，将渲染图像和真实图像结合起来，生成一个具有ground truth NOCS图、mask和类别标签的合成图。作者总共渲染了30万张图片。

![image-20211024020510341](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc2q91byj61eo0c4aed02.jpg)

> **Notes：**
>
> ShapeNet
>
> 自动标注ground truth物体位姿和带下的方法

#### 模型

作者的网络结构是基于Mask R-CNN框架构建的，增加了预测NOCS图的分支。RGB图和深度图作为输入，CNN通过RGB图预测物体的类别标签、mask和NOCS图，之后将NOCS图与深度图进行拟合得到物体的6D位姿和大小（作者在CNN中没有使用深度图，因为作者使用COCO数据集来提高网络的鲁棒性，而COCO不包含深度图）。

![image-20211024020941066](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc2tgglej61aw0elq5s02.jpg)

作者增加的分支分别预测NOCS图的x、y、z坐标，通过回归每个像素值或者离散化像素值，把它当做一个分类问题。实验表明，图4中B=32的像素分类比直接回归更好

![image-20211024021007230](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc2wnkzcj61060elwhf02.jpg)

#### 6D位姿和尺寸估计

作者的目标是通过使用NOCS图和深度图来估计被检测物体的6D位姿和大小。为此，作者使用RGB-D相机内参和外参来将深度图像与彩色图像对齐，使用预测的物体mask来获得物体的3D点云Pm，使用NOCS图来获得预测位姿Pn。然后，估计将Pn转换为Pm的比例、旋转和平移。对于这个7维刚性变换估计问题，作者使用Umeyama算法，而对于离群点去除，作者使用RANSAC。

#### 实验结果

作者使用IoU来评估三维目标检测和尺寸的估计，使用平均精度来评估平移误差小于m厘米，旋转误差小于n°的物体位姿估计。将目标检测与位姿估计解耦，将检测阈值设为10%来保证大部分物体都包含在评估中。因为不知道其他类别级6D位姿和大小估计的方法，所以作者使用Mask RCNN+ICP建立baseline来帮助比较性能。

在合成数据集（CAMERA*）上测试:对于50%的3D IoU，mAP为83.9%，位姿使用(5cm，5°)度量，mAP为40.9%。

![image-20211024021213153](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc30q068j60ht08kdgs02.jpg)

在真实数据集（REAL）上测试:在COCO的弱监督下，使用CAMERA* 与REAL* 共同训练网络，并在真实世界的测试集中对其进行评估。由于COCO没有ground truth NOCS图，在训练中不使用NOCS损失。为了平衡这些数据集，作者从三个数据源中为每个小批次选择图像，CAMERA* 的概率为60%，COCO 为20%，REAL*为20%。对于50%的3D IoU， mAP为76.4%，位姿使用(5cm，5°)，mAP为10.2%，使用(5cm，10°) ，mAP为23.1%。相比之下，baseline在50%的3D IoU时，mAP为43.8%，而(5cm，5°)和(5cm，10°)的mAP为0.8%，明显低于本文的性能。

![image-20211024021311558](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc338nrhj60i608nwfi02.jpg)

作者还创建了一个CAMERA* 的变体，其中图像是以非上下文感知的方式合成的(在表中由B表示)。如表中所示，仅在REAL* 或REAL* 和COCO上进行培训，由于数据集较小，会产生过拟合。CAMERA* 与COCO和REAL* 一起进行训练，可以获得最佳效果。

![image-20211024021334643](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc36i3tsj60ia09fmy102.jpg)

作者给出了5个不同指标的AP，其中3D25和3D25分别代表25%和50%的3D IoU。

作者还与PoseCNN进行了比较。使用2D重投影误差（测量ground truth和估计目标位姿之间的平均像素距离小于5个像素），作者的方法2D重投影误差的mAP为30.2%。PoseCNN 2D重投影误差的mAP为17.2%。

![image-20211024021418345](https://tva1.sinaimg.cn/large/008i3skNgy1gvrc39kfphj611f0ergp202.jpg)

#### 不足

存在缺失检测、错误分类和预测坐标图不一致。

### Paper4：Deep-6DPose: Recovering 6D Object Pose from a Single RGB Image

还在读ing

### 主流6D位姿估计方法总结：

+ **需要每一个物体CAD实例（Instance-Level）**
  + DenseFusion: 6D Object Pose Estimation by Iterative Dense Fusion(https://arxiv.org/abs/1901.04780)
  + PVN3D: A Deep Point-wise 3D Keypoints Voting Network for 6DoF Pose Estimation(https://arxiv.org/abs/1911.04231)
  + FFB6D: A Full Flow Bidirectional Fusion Network for 6D Pose Estimation(https://arxiv.org/abs/2103.02242)
+ **需要一类物品的CAD模型（Category-Level）**
  + NOCS：Normalized Object Coordinate Space for Category-Level 6D Object Pose and Size Estimation(https://arxiv.org/abs/1901.02970)（开源）
  + 6-PACK: Category-level 6D Pose Tracker with Anchor-Based Keypoints(https://arxiv.org/abs/1910.10750)（开源）
  + Shape Prior Deformation for Categorical 6D Object Pose and Size Estimation(https://arxiv.org/abs/2007.08454)（开源）
  + Category Level Object Pose Estimation via Neural Analysis-by-Synthesis(https://arxiv.org/abs/2008.08145)

+ **不需要CAD模型，只是采用RGB-D或是纯粹RGB图像**（较少）
  + Deep-6DPose: Recovering 6D Object Pose from a Single RGB Image(https://arxiv.org/abs/1802.10367v1)（程序暂时未开源）
  + BundleTrack: 6D Pose Tracking for Novel Objects without Instance or Category-Level 3D Models(https://arxiv.org/abs/2108.00516)（开源）


### Reference
- [1] [BundleTrack论文](https://arxiv.org/abs/2108.00516)
- [2] [6-PACK论文](https://arxiv.org/abs/1910.10750)
- [3] [NOCS论文](https://arxiv.org/abs/1901.02970)
- [4] [6D位姿估计方法总结](https://zhuanlan.zhihu.com/p/367419191)
- [5] [6D姿态估计算法汇总](https://zhuanlan.zhihu.com/p/101331713?from_voters_page=true)
- [5] [物体6D位姿估计](https://www.zhihu.com/column/c_1151970556306874368)