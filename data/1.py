import os
import shutil

L = [
'/home/coco/datasets/coco/uav/images/val2017',
'/home/coco/datasets/coco/uav/images/train2017'
]
# ["Person","Car","Truck","bus","Animal","fire","smoke"]
# ["Person","Car","fire","smoke","Animal"]
cls_map = {'0':'0','1':'1','2':'1','3':'1','4':'4','5':'2','6':'3'}
def readfiles(p, exts=['jpg', 'jpeg', 'png']):
    L = []
    for root, dir, files in os.walk(p):
        for f in files:
            if f.split('.')[-1].lower() in exts:
                L.append(os.path.join(root, f))
    return L

for l in L:
    label_path = l.replace('images','labels')
    new_label_path = l.replace('images','labels1')
    if os.path.exists(new_label_path):
        shutil.rmtree(new_label_path)
    if not os.path.exists(new_label_path):
        os.makedirs(new_label_path)

    label_list = readfiles(label_path,['txt'])
    for label in label_list:
        if label.find('classes.txt') > -1:
            continue
        new_label = label.replace('labels','labels1')
        new_label_w = open(new_label,'a')
        with open(label,'r') as f:
            lines = f.readlines()
            for line in lines:
                line = line.strip().split()
                cls = line[0]
                # try:
                #     cls = cls_map[cls]
                # except:
                #     print(label)
                if cls not in ['0','1']:
                    continue
                st = cls+' '+' '.join(line[1:])+'\n'
                new_label_w.write(st)
        new_label_w.close()
     
    