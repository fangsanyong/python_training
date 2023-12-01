
import os
import shutil

def readfiles(p, exts=['jpg', 'jpeg', 'png']):
    L = []
    for root, dir, files in os.walk(p):
        for f in files:
            if f.split('.')[-1].lower() in exts:
                L.append(f)
    return L

bi_p = '/home/coco/datasets/ped-car-temp/bird/20231122/coco_bg_person/images/train2017'
bl_p = '/home/coco/datasets/ped-car-temp/bird/20231122/coco_bg_person/labels/train2017'
if os.path.exists(bi_p):
        shutil.rmtree(bi_p)
if not os.path.exists(bi_p):
    os.makedirs(bi_p)

if os.path.exists(bl_p):
        shutil.rmtree(bl_p)
if not os.path.exists(bl_p):
    os.makedirs(bl_p)
    
l = '/home/coco/datasets/coco/labels_old/train2017'
fs = readfiles(l,['txt'])
for i in fs :
    # cur = []
    count = 0 
    new_l = os.path.join(bl_p,i) 
    new_i = os.path.join(bi_p,i.replace('txt','jpg'))
    i = os.path.join(l, i)
    image_p = i.replace('labels','images').replace('txt','jpg')
    with open(i,'r') as f:
        lines = f.readlines()
        for line in lines :
            line = line.strip().split()
            if line[0] == '0':
                count += 1
            if line[0] == '14':
                count = 0
                break
            # line[0] = '0'
            # cur.append(' '.join(line) + '\n')
            
    if count !=0:
        shutil.copyfile(image_p,new_i)
        ss = open(new_l,'a')
        # for c in cur:
        #     ss.write(c)
        ss.close()
        