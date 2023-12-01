def img2label_paths(img_path):
    # Define label paths as a function of image paths
    sa, sb = os.sep + 'images' + os.sep, os.sep + 'labels' + os.sep  # /images/, /labels/ substrings
    return sb.join(img_path.rsplit(sa, 1)).rsplit('.', 1)[0] + '.txt'


import os

with open('3.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip()
        label = img2label_paths(line)
        if os.path.exists(line):
            os.remove(line)
        if os.path.exists(label):
            os.remove(label)
