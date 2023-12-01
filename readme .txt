用Docker搭建yolov5开发环境

1. 安装Docker

2. 下载yolov5代码 git clone https://github.com/ultralytics/yolov5.git（不行就自己下载一个）
3. docker操作

（1）docker pull ubuntu:18.04
（2）docker run -it --name="my_container" IMAGE_ID  /bin/bash
  (3)  exit 退出容器  
       docker rm 容器ID  删除容器
       docker start container_id 
      docker restart container_id
      docker attach container_id
      docker cp my_container:/home/yolov5  ./yolov5  从容器拷贝文件到主机
      docker cp  ./yolov5 my_container:/home/yolov5   从主机拷贝文件到容器
（4）打包保存新的镜像
         docker save ubuntu > my_ubuntu.tar
         docker load < my_ubuntu.tar
  （5）删除镜像
       docker images
       docker rmi  -f  IMAGE_ID  删除镜像
       docker images
       docker load -i my_ubuntu.tar
       docker images
       docker start container_id/容器名
       docker attach container_id/容器名
       



3. 构建Docker镜像

