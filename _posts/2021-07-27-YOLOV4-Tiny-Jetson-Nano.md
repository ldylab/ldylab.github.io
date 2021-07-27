---
layout:     post                    # 使用的布局（不需要改）
title:      Jetson Nano部署YoloV4-Tiny              # 标题 
subtitle:   YoloV4-Tiny & Jetson Nano  #副标题
date:       2021-07-27              # 时间
author:     Henry                      # 作者
header-img: img/post-bg-unix-linux.jpg    #这篇文章标题背景图片
catalog: true                       # 是否归档
tags:                               #标签
    - Technology
---
> 对于目标检测识别，Yolo系列一直是工业与学术常用框架，YoloV4相较于V3有了很大的提升，而对于Jetson Nano其优秀的体积、极低的功耗非常适用于嵌入式开发平台的在实际项目中的运用，在此使用Jetson Nano部署YoloV4-Tiny框架，并利用Tensor RT实现对于识别过程的加速运算。

### YoloV4环境部署

+ 首先要先建置darknet的环境，先下载darknet的github：

  ```shell
  $ git clone https://github.com/AlexeyAB/darknet.git
  $ cd darknet
  ```

+ 修改cmake文档，官方文档有提供TX1/2的修改方法，Jetson Nano一致即可：

  ```cmake
  GPU=1
  CUDNN=1
  CUDNN_HALF=1
  OPENCV=1
  AVX=0
  OPENMP=1
  LIBSO=1
  ZED_CAMERA=0
  ZED_CAMERA_v2_8=0
  
  ......
  
  USE_CPP=0
  DEBUG=0
  
  ARCH= -gencode arch=compute_53,code=[sm_53,compute_53]
  ```

+ 构建：

  ```cmake
  make
  ```

+ 可能遇到nvcc问题：

  ![image-20210727102556827](https://tva1.sinaimg.cn/large/008i3skNgy1gsvbu0oyalj6128054dh102.jpg)

  首先查看nvcc驱动是否正常，一般使用官方的镜像都是没问题的：

  ```shell
  nvcc --version
  ```

  可以直接在Cmake中添加NVCC的绝对位置即可：

  ![image-20210727102659033](https://tva1.sinaimg.cn/large/008i3skNgy1gsvbtyoqrxj314y076gmt.jpg)

### 使用YOLOv4进行推论

+ 下载权重：

  ```shell
  wget https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v3_optimal/yolov4.weights \
         -q --show-progress --no-clobber
  ```

+ 基本的推论方法有三种：图片、影片、摄像头，主要执行除了darknet的执行档之外还需要给予模式、资料集、配置档：

  ```shell
  ./darknet detector test ./cfg/coco.data ./cfg/yolov4.cfg ./yolov4.weights
  ```

+ 可以使用--help来帮助查看：

  ```shell
  ./darknet detector --help
  ```

+ 如果要开启图片的话需使用test模式，在执行之后要你输入图片的位置，按任意建离开后，图片不会储存：

  ```shell
  ./darknet detector test cfg/coco.data cfg/yolov4.cfg yolov4.weights -thresh 0.25
  ```

+ 如果想要指定图片并且将结果储存下来则可以增加-ext_output 的选项，执行完会储存成prediction.jpg，如：

  ```shell
  ./darknet detector test cfg/coco.data cfg/yolov4.cfg yolov4.weights -ext_output Taiwan.jfif
  ```

  ![image-20210727103206177](https://tva1.sinaimg.cn/large/008i3skNgy1gsvbu3p6jfj31900u0ap0.jpg)

+ 如果要使用影片是使用demo 的指令来操作，这边如果用-ext_output会直接覆盖掉原本的，我希望可以另存成别的文件则需要用到-output_filename来执行：

  ```shell
  ./darknet detector demo cfg/coco.data cfg/yolov4.cfg yolov4.weights sample.mp4 -out_filename sample_.mp4
  ```

+ 使用摄像头实时辨识需要在后面参数导入-c：

  ```shell
  $ ./darknet detector demo cfg/coco.data \
                            cfg/yolov4.cfg \
                            yolov4.weights \
                            -c 0
  ```

+ 上述测试结论：

  对于Jetson Nano其使用YoloV4的实时识别帧率在0.8FPS左右。

+ 修改输入尺寸提高识别的帧率：

  我们也可以直接修改输入输出的图片大小，我用简单一点的语法来操作，复制一个yolov4.cfg并命名为yolov4-416.cfg，并直接用nano去修改输入大小成416，这边使用&&的意思是让前一个指令完成之后再接续下一个指令：

  ```shell
  cp cfg/yolov4.cfg cfg/yolov4-416.cfg && nano cfg/yolov4-416.cfg
  ```

  在修改之后其帧率从0.8提高至1.5左右，但是官方会比较推荐输入尺寸在608以上。

### 使用结构更小的YOLO ( Yolov4-Tiny )

+ 下一种加快速度的方法是使用yolov4-tiny.weights，一个更小型的yolov4，这边的小型指的是神经网路模型的结构，其牺牲很小的精度但获得更高的帧率，在实际使用时先下载其权重：

  ```shell
  wget https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4-tiny.weights
  ```

+ 接着使用摄像头来开启，注意这边的config (cfg) 档案需要更改成yolov4-tiny.cfg，因为yolov4跟yolov4-tiny的架构有所不同，config文件当中所提供的就是神经网路的结构：

  ```shell
  $ ./darknet detector demo cfg/coco.data \
                            cfg/yolov4-tiny.cfg \
                            yolov4-tiny.weights \
                            -c 0
  ```

+ 测试结果：

  实时帧率在14fps有了很大的提升，不过对于精度对于下降个10%左右，对于项目可完全可接受的。

### 使用TensorRT引擎加速

接下来是TensorRT的版本，Tensor RT是一个加速引擎，可以运用在有CUDA核心的NVIDIA显卡当中，如果要使用TRT引擎加速需要先将神经网路模型转换成ONNX的格式才行。

#### 下载、安装环境

+ 已经有很多人利用Yolov4做了很多应用，而转换这块也已经有人完成了，所以直接使用Github上来实现即可：
  ```shell
  $ git clone https://github.com/jkjung-avt/tensorrt_demos.git
  ```

+ 下载后直接执行：
  ```shell
  $ cd ${HOME}/project/tensorrt_demos/ssd
  $ ./install_pycuda.sh
```

+ 将iffi中间的内容修改如下:
  ```shell
  nano ./install_pycuda.sh
  ```
  ![image-20210727104752584](https://tva1.sinaimg.cn/large/008i3skNgy1gsvbdedx2kj31hc0dcq4w.jpg)

+ 安装完之后应该会显示finished processing dependencies，也可以使用pip3 list去查看pycuda是否有安装成功.

+ 接着需要安装onnx，一开始先安装相依套件，接着在安装onnx 1.4.1版本：

  ```shell
  $ sudo apt-get install protobuf-compiler libprotoc-dev
  $ sudo pip3 install onnx==1.4.1
  ```

+ 构建：

  ```shell
  $ cd ${HOME}/project/tensorrt_demos/plugins
  $ make
  ```

#### 下载并转换yolo模型

+ 接着需要下载模型的权重，它下载了yolo3跟yolo4的三种不同版本，并且直接放在当前资料夹当中，这边可以注意到下载的模型与刚刚的YOLOv4相同，所以其实也是可以直接用复制的方式或是直接写绝对位置进行转换：

  ```shell
  $ cd ${HOME}/project/tensorrt_demos/yolo
  $ ./download_yolo.sh
  ```

+ 最后可以执行yolo_to_onnx.py 将yolo的权重档转换成onnx文件，接着再编译成TRT可用的模型，在onnx_to_tensorrt.py最好使用-v 来看到进度，不然看着画面没动静会有点紧张：

  ```shell
  $ python3 yolo_to_onnx.py -m yolov4-416
  $ python3 onnx_to_tensorrt.py -m yolov4-416 -v
  ```

+ 转换ONNX大约需要15分钟，会储存成yolov4-416.onnx，接着转换TRT大概也是差不多的时间，最后会储存成yolov4-416.trt。

### 使用TRT运行YOLOv4-416

+ 直接跑USB摄像头，这边使用--usb 代表使用USB摄影机， --model则是选择特定模型：

  ```shell
  $ cd ${HOME}/project/tensorrt_demos
  $ python3 trt_yolo.py --usb 0 --model yolov4-416
  ```

+ 结果：使用416维度图片，获得了4.2~4.5左右的帧率，相比于没有使用TensorRT提高了3倍的帧率。

### 使用TRT运行YOLOv4-Tiny-416

+ 接下来为了更快的速度，当然要实测一下tiny版本的：

  ```shell
  $ python3 yolo_to_onnx.py -m yolov4-tiny-416
  $ python3 onnx_to_tensorrt.py -m yolov4-tiny -416 -v
  $ cd ${HOME}/project/tensorrt_demos
  $ python3 trt_yolo.py --usb 0 --model yolov4-tiny-416
  ```

  

+ 结果：使用416维度图片，获得18左右的帧率，相比于没有使用TensorRT提高了4fps倍的帧率，还是有着非常棒的效果，不过其辨别效果相比于YoloV4下降一些，但也足够项目需求。

$\begin{aligned} \text { elbow }_{x} &=l_{0} \cos \left(\theta_{0}\right) \\ \text { elbow }_{y} &=l_{o} \sin \left(\theta_{0}\right) \\ \text { wrist }_{x} &=\text { elbow }_{x}+l_{1} \cos \left(\theta_{0}+\theta_{1}\right)=l_{0} \cos \left(\theta_{0}\right)+l_{1} \cos \left(\theta_{0}+\theta_{1}\right) \\ \text { wrist }_{y} &=\text { elbow }_{y}+l_{1} \sin \left(\theta_{0}+\theta_{1}\right)=l_{0} \sin \left(\theta_{0}\right)+l_{1} \sin \left(\theta_{0}+\theta_{1}\right) \end{aligned}$

### Reference

- [1] [DarkNet Yolo Github](https://github.com/AlexeyAB/darknet)

- [2] [Tensor RT](https://github.com/jkjung-avt/tensorrt_demos#yolov4)
