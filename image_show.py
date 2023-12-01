import os
import cv2


def img2label_paths(img_path):
    # Define label paths as a function of image paths
    sa, sb = os.sep + 'images' + os.sep, os.sep + 'labels' + os.sep  # /images/, /labels/ substrings
    return sb.join(img_path.rsplit(sa, 1)).rsplit('.', 1)[0] + '.txt'


# 红外
# classes = ["Person", "Car", "fire", "smoke"]
classes = ["bird"]
# rgb
# classes = ["Person", "Car", "Truck", "bus", "Animal", "fire", "smoke"]

# 设置文件路径
p = '/home/coco/datasets/coco/bird/images/train2017'
L = []
for root, dir, files in os.walk(p):
    for f in files:
        if f.split('.')[-1] in ['jpg', 'jpeg','png']:
            L.append(os.path.join(root, f))

aa = open('3.txt', 'a')
for l in L:
    print(l)
    label = img2label_paths(l)  # l.replace('images', 'labels').replace('.jpg', '.txt')
    if not os.path.exists(label):
        aa.write(l + '\n')
        continue
    image = cv2.imread(l)
    shape = image.shape
    h, w = shape[:2]
    with open(label, 'r') as f:
        lines = f.readlines()
        # if len(lines) == 0:
        #     print(label)
        #     aa.write(l + '\n')
        #     continue
        for line in lines:
            line = line.strip().split()
            line = [float(i) for i in line]
            cls = int(line[0])
            cls = classes[cls]
            l1 = int((line[1] - line[3] / 2) * w)
            t = int((line[2] - line[4] / 2) * h)
            r = int((line[1] + line[3] / 2) * w)
            d = int((line[2] + line[4] / 2) * h)
            cv2.rectangle(image, (l1, t), (r, d), (0, 0, 255), 1)
            cv2.putText(image, cls, (l1 - 2, t - 2), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 0, 255), 1)
    cv2.imshow('1', image)
    if cv2.waitKey(0) & 0xFF == ord('q'):
        with open('3.txt', 'a') as aa:
            aa.write(l + '\n')
    # cv2.waitKey(0)
aa.close()
