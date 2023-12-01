卸载docker

步骤一：停止Docker

sudo systemctl stop docker

步骤二：删除Docker软件包

sudo apt-get purge docker-ce

步骤三：删除Docker目录和文件

sudo rm -rf /var/lib/docker
sudo rm -rf /etc/docker
sudo rm /etc/apparmor.d/docker
sudo groupdel docker

在终端上，输入以下命令以删除Docker存储库：

sudo apt-get remove docker-ce-cli
sudo apt-get autoremove -y

您可以使用以下命令来验证是否已成功卸载。
docker --version
如果输出结果类似于“docker command not found”，则表示已成功删除。

结论：以上是彻底卸载Docker的步骤。在出现问题时，建议先进行这些步骤，彻底删除Docker并重新安装它，以保障Docker的正确性和稳定性。

#########################################################################################################################################

安装Docker

1、查看是否已经安装docker
docker --version
sudo apt-get remove docker docker-engine docker.io containerd runc

2、sudo apt-get update

3、允许apt命令可以使用HTTP访问Docker repository

sudo apt-get install apt-transport-https ca-certificates curl gnupg-agent software-properties-common

4、添加docker官方GPG key

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
返回 OK
验证key

清华源
sudo add-apt-repository "deb [arch=amd64] https://mirrors.tuna.tsinghua.edu.cn/docker-ce/linux/ubuntu $(lsb_release -cs) stable"
或者阿里源
sudo add-apt-repository "deb [arch=amd64] https://mirrors.aliyun.com/docker-ce/linux/ubuntu $(lsb_release -cs) stable"

5、更新源

sudo apt-get update

6、安装docker-ce软件

sudo apt-get install docker-ce docker-ce-cli containerd.io

7、重启docker

service docker restart

8、测试

sudo docker run hello-world

9、 查看docker版本

sudo docker version

10、查看镜像，可以看到刚才创建的镜像

sudo docker images
###################################################################################################################
命令补全

通过bash_complete，docker提供自动补全功能，在执行该命令时，按tab即可自动补全参数，提高命令输入效率

1、安装bash_completion

sudo apt install bash-completion

2、加载bash_completion

source /etc/bash_completion

####################################################################################################################
更改默认存储位置

1、查看docker数据存储路径

sudo docker info

2、查看docker数据占用的存储空间(-v参数是详细列出)

sudo docker system df
sudo docker system df -v

3. 新建配置文件并输入以下内容

sudo vim /etc/docker/daemon.json  
{
//"data-root": "/home/xxx/docker", # 新的路径
"registry-mirrors": ["http://hub-mirror.c.163.com","https://docker.mirrors.ustc.edu.cn","https://registry.docker-cn.com"]
}

4、将原来docker中的数据复制到新的存储目录下

sudo cp -r /var/lib/docker /home/zhuoyu/fsy/docker

5、重启Docker

sudo systemctl restart docker

6、查看image信息

docker images

7、删除之前的数据

rm -rf /var/lib/docker

#######################################################
国内镜像

1.Docker中国区官方镜像
https://registry.docker-cn.com

2.网易
http://hub-mirror.c.163.com

3.中科大
https://docker.mirrors.ustc.edu.cn
##################################################################
https://cloud.tencent.com/document/product/1643/86490
https://zhuanlan.zhihu.com/p/459382093
https://blog.csdn.net/weixin_44795604/article/details/131039795


查看docker状态
systemctl status docker

启动docker
systemctl start docker

停止docker
systemctl stop docker

重启docker
systemctl restart docker


镜像命令
列出本地主机上的镜像
docker images 

删除某个XXX镜像名字ID
删除单个
docker rmi  -f  镜像ID


新建+启动容器
 docker run --name="容器名称"  IMAGE [COMMAND] [ARG...]  

使用镜像centos:latest以交互模式启动一个容器,在容器内执行/bin/bash命令。
docker run -it centos /bin/bash  

列出当前所有正在运行的容器
docker ps -a

停止容器
docker stop 容器ID或者容器名

强制停止容器
docker kill 容器ID或容器名

在删除之前需要先停止容器，然后执行以下命令
docker rm 容器ID

强制删除容器，容器运行时可以直接删除
docker rm -f 容器ID

##################################################################
https://www.python100.com/html/79388.html

Docker镜像导出详解
一、Docker镜像导出与导入
Docker镜像可以被导出为一个tar包，并可以通过Docker的import命令导入一个Docker镜像。
docker save hello-world > hello-world.tar
docker load < hello-world.tar
上面的命令将hello-world镜像导出为一个tar包，并通过load命令导入。










