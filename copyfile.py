import os
import shutil


p = '/home/coco/datasets/coco/labels/train2017'
L = []
for root, dir, files in os.walk(p):
    for f in files:
        if f.split('.')[-1] in ['txt']:
            L.append(os.path.join(root, f))

for l in L :
    image_path = l.replace('labels','images').replace('.txt','.jpg')
    new_image_path = l.replace('labels','images1').replace('.txt','.jpg')
    shutil.copyfile(image_path,new_image_path)