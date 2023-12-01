import os



cls_map =  {0:0,2:1,5:3,7:2,14:4,15:4,16:4,17:4,18:4,19:4,20:4,21:4,22:4,23:4}
p = '/home/coco/datasets/coco/labels/val2017'
L = []
for root, dir, files in os.walk(p):
    for f in files:
        if f.split('.')[-1] in ['txt']:
            L.append(os.path.join(root, f))


for l in L:
    label = l.replace('labels','labels1')
    with open(l,'r') as f :
        lines = f.readlines()
        for line in lines:
            line = line.strip().split()
            cls = int(line[0])
            other = ' '.join(line[1:])
            if cls in cls_map.keys():
                cls = cls_map[cls]
            else:
                continue
            st = str(cls)+' '+other
            with open(label,'a') as p:
                p.write(st+'\n')

    
            