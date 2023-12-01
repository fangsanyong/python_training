import os


K = [
#    '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/1/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/2/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/3/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/4/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/5/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/6/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/7/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/8/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/9/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/10/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/11/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/20230815/gdy/12/images', # 固定翼
# '/home/coco/datasets/ped-car-temp/uav/buy/1/fix/images', # 无人机数据 固定翼
# '/home/coco/datasets/ped-car-temp/uav/buy/3/fix/images', # 无人机数据 固定翼
'/home/coco/datasets/coco/bird/images/train2017'

]

def readfiles(p, exts=['bmp', 'dng', 'jpeg', 'jpg', 'mpo', 'png', 'tif', 'tiff', 'webp']):
    L = []
    for root, dir, files in os.walk(p):
        for f in files:
            if f.split('.')[-1].lower() in exts:
                L.append(os.path.join(root, f))
    return L


count = 0 
for i in K:
    ss = readfiles(i)
    count += len(ss)
    print(len(ss))
print(count)
# 32755