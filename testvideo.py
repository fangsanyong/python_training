import argparse
import glob
import json
import os
import os.path
import time
from pathlib import Path
 
import numpy as np
import onnxruntime
import tqdm
import cv2
import torch
import torchvision
 
# set model
CONFIG = 4
if CONFIG == 4:
    IMAGE_SIZE = (320, 224)
    CONF_TH = 0.6
    NMS_TH = 0.45
    CLASSES = 5
    #model = onnxruntime.InferenceSession("0630.onnx", None, providers=["CUDAExecutionProvider"])
    model = onnxruntime.InferenceSession("/home/coco/yolov5-6.1/runs/train/exp16/weights/best.onnx", None, providers=["CUDAExecutionProvider"])
    is_ones = False
# set print
# np.set_printoptions(threshold=np.inf)
 
# utils_____________________________________________________________________________________________
 
# 画一个框
def plot_one_box(x, img, color=None, label=None):
    c1, c2 = (int(x[0]), int(x[1])), (int(x[2]), int(x[3]))
    cv2.rectangle(img, c1, c2, color, 2)
 
    # cv2.rectangle(img, (int(x[0]), int(x[1]) + 35),
    #               (int(x[0]) + 100, int(x[1]) + 2), (128, 128, 128), -1)
    cv2.putText(img, label, (int(x[0]), int(
        x[1]) + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), thickness=1, lineType=cv2.LINE_AA)
 
 
# 查看是否越界
def clip_coords(boxes, img_shape):
    # Clip bounding xyxy bounding boxes to image shape (height, width)
    boxes[:, 0].clip(0, img_shape[1])  # x1
    boxes[:, 1].clip(0, img_shape[0])  # y1
    boxes[:, 2].clip(0, img_shape[1])  # x2
    boxes[:, 3].clip(0, img_shape[0])  # y2
 
 
def draw(img, boxinfo, dst, id):
    is_exsit = results is not None and len(results)
    if is_exsit:
        for *xyxy, conf, cls in boxinfo:
            label = '{}|{}'.format(int(cls), '%.2f' % conf)
            plot_one_box(xyxy, img, color=[0, 0, 255], label=label)
    # cv2.imencode('.jpg', img)[1].tofile(dst)
    return img
 
 
def xywh2xyxy(x):
    y = np.zeros_like(x)
    y[:, 0] = x[:, 0] - x[:, 2] / 2  # top left x
    y[:, 1] = x[:, 1] - x[:, 3] / 2  # top left y
    y[:, 2] = x[:, 0] + x[:, 2] / 2  # bottom right x
    y[:, 3] = x[:, 1] + x[:, 3] / 2  # bottom right y
    return y
 
 
# nms numpy
def box_area(boxes):
    """
    :param boxes: [N, 4]
    :return: [N]
    """
    return (boxes[:, 2] - boxes[:, 0]) * (boxes[:, 3] - boxes[:, 1])
 
 
def box_iou(box1, box2):
    """
    :param box1: [N, 4]
    :param box2: [M, 4]
    :return: [N, M]
    """
    area1 = box_area(box1)  # N
    area2 = box_area(box2)  # M
    # broadcasting, 两个数组各维度大小 从后往前对比一致， 或者 有一维度值为1；
    lt = np.maximum(box1[:, np.newaxis, :2], box2[:, :2])
    rb = np.minimum(box1[:, np.newaxis, 2:], box2[:, 2:])
    wh = rb - lt
    wh = np.maximum(0, wh)  # [N, M, 2]
    inter = wh[:, :, 0] * wh[:, :, 1]
    iou = inter / (area1[:, np.newaxis] + area2 - inter)
    return iou  # NxM
 
 
def numpy_nms(boxes, scores, iou_threshold):
    idxs = scores.argsort()  # 按分数 降序排列的索引 [N]
    keep = []
    while idxs.size > 0:  # 统计数组中元素的个数
        max_score_index = idxs[-1]
        max_score_box = boxes[max_score_index][None, :]
        keep.append(max_score_index)
 
        if idxs.size == 1:
            break
        idxs = idxs[:-1]  # 将得分最大框 从索引中删除； 剩余索引对应的框 和 得分最大框 计算IoU；
        other_boxes = boxes[idxs]  # [?, 4]
        ious = box_iou(max_score_box, other_boxes)  # 一个框和其余框比较 1XM
        idxs = idxs[ious[0] <= iou_threshold]
 
    keep = np.array(keep)  # Tensor
    return keep
# end nms numpy
 
 
def nms(prediction, conf_thres=0.75, iou_thres=0.45, classes=None, agnostic=False, multi_label=False,labels=(), max_det=300):
    """Runs Non-Maximum Suppression (NMS) on inference results
 
    Returns:
         list of detections, on (n,6) tensor per image [xyxy, conf, cls]
    """
 
    nc = prediction.shape[2] - 5  # number of classes
    xc = prediction[..., 4] > conf_thres  # candidates
 
    # Checks
    assert 0 <= conf_thres <= 1, f'Invalid Confidence threshold {conf_thres}, valid values are between 0.0 and 1.0'
    assert 0 <= iou_thres <= 1, f'Invalid IoU {iou_thres}, valid values are between 0.0 and 1.0'
 
    # Settings
    # (pixels) minimum and maximum box width and height
    min_wh, max_wh = 2, 4096
    max_nms = 30000  # maximum number of boxes into torchvision.ops.nms()
    time_limit = 10.0  # seconds to quit after
    redundant = True  # require redundant detections
    multi_label &= nc > 1  # multiple labels per box (adds 0.5ms/img)
    merge = False  # use merge-NMS
 
    t = time.time()
    output = [np.zeros((0, 6))
              ] * prediction.shape[0]
 
    for xi, x in enumerate(prediction):  # image index, image inference
        # Apply constraints
        # x[((x[..., 2:4] < min_wh) | (x[..., 2:4] > max_wh)).any(1), 4] = 0  # width-height
        x = x[xc[xi]]  # confidence
 
        # Cat apriori labels if autolabelling
        if labels and len(labels[xi]):
            l = labels[xi]
            v = np.zeros((len(l), nc + 5))
            v[:, :4] = l[:, 1:5]  # box
            v[:, 4] = 1.0  # conf
            v[range(len(l)), l[:, 0].long() + 5] = 1.0  # cls
            x = np.cat((x, v), 0)
 
        # If none remain process next image
        if not x.shape[0]:
            continue
 
        # Compute conf
        x[:, 5:] *= x[:, 4:5]  # conf = obj_conf * cls_conf
 
        # Box (center x, center y, width, height) to (x1, y1, x2, y2)
        box = xywh2xyxy(x[:, :4])
        # Detections matrix nx6 (xyxy, conf, cls)
        if multi_label:
            i, j = (x[:, 5:] > conf_thres).nonzero(as_tuple=False).T
            x = np.concatenate(
                (box[i], x[i, j + 5, None], j[:, None].float()), 1)
        else:  # best class only
            conf = x[:, 5:]
            j = np.zeros((conf.shape[0], 1), float)
            x = np.concatenate((box, conf, j), 1)
 
        # Filter by class
        if classes is not None:
            x = x[(x[:, 5] == classes)]
 
        # Apply finite constraint
        # if not torch.isfinite(x).all():
        #     x = x[torch.isfinite(x).all(1)]
 
        # Check shape
        n = x.shape[0]  # number of boxes
        if not n:  # no boxes
            continue
        elif n > max_nms:  # excess boxes
            # sort by confidence
            x = x[x[:, 4].argsort(descending=True)[:max_nms]]
 
        # Batched NMS
        c = x[:, 5:6] * (0 if agnostic else max_wh)  # classes
        # boxes (offset by class), scores
        boxes, scores = x[:, :4] + c, x[:, 4]
        i = numpy_nms(boxes, scores, iou_thres)  # NMS
        if i.shape[0] > max_det:  # limit detections
            i = i[:max_det]
        if merge and (1 < n < 3E3):  # Merge NMS (boxes merged using weighted mean)
            # update boxes as boxes(i,4) = weights(i,n) * boxes(n,4)
            iou = box_iou(boxes[i], boxes) > iou_thres  # iou matrix
            weights = iou * scores[None]  # box weights
            x[i, :4] = np.float(np.matmul(weights, x[:, :4])) / \
                weights.sum(1, keepdim=True)  # merged boxes
            if redundant:
                i = i[iou.sum(1) > 1]  # require redundancy
 
        output[xi] = x[i]
        if (time.time() - t) > time_limit:
            print(f'WARNING: NMS time limit {time_limit}s exceeded')
            break  # time limit exceeded
 
    return output
 
 
def creatdict(imgpath, persons):
    params = {"image_path": imgpath,"results": persons}
    return params
 
 
def imread(path) -> np.ndarray:
    stream = open(path, "rb")
    bytes = bytearray(stream.read())
    numpyarray = np.asarray(bytes, dtype=np.uint8)
    bgrImage = cv2.imdecode(numpyarray, cv2.IMREAD_UNCHANGED)
    return bgrImage
 
 
def get_one_class(pred, clss):
    bbox = pred[0][0][:, :5]
    pcls = pred[0][0][:, clss+5]
    pcls = pcls.reshape(pcls.shape[0], 1)
    pp = np.array([np.concatenate((bbox, pcls), axis=1)])
    return pp
# END utils_________________________________________________________________________________________
 
 
def detect(image):
    height, width = image.shape[:2]
    if (width / height) >= (IMAGE_SIZE[0] / IMAGE_SIZE[1]):
        lenmb = int(width*IMAGE_SIZE[1] / IMAGE_SIZE[0]-height)
        img = cv2.copyMakeBorder(image, 0, lenmb, 0, 0, cv2.BORDER_CONSTANT, value=[114, 114, 114])
        img = cv2.resize(img, IMAGE_SIZE)
        w,h = [IMAGE_SIZE[0],int(IMAGE_SIZE[0]*height/width)]
    else:
        lenmb = int(height*IMAGE_SIZE[0] / IMAGE_SIZE[1]-width)
        img = cv2.copyMakeBorder(image, 0, 0, 0, lenmb, cv2.BORDER_CONSTANT, value=[114, 114, 114])
        img = cv2.resize(img, IMAGE_SIZE)
        h,w = [IMAGE_SIZE[1],int(IMAGE_SIZE[1]*width/height)]
    
    img_s = image.copy()
    # print(w,h)
    # cv2.imshow('1',img)
    # cv2.waitKey(0)
    img = img.transpose(2, 0, 1)[::-1]
    img = np.ascontiguousarray(img)
    img = img.astype('float32')
    img /= 255.0
    img = img.reshape(1, 3, IMAGE_SIZE[1], IMAGE_SIZE[0])
    inputs = {model.get_inputs()[0].name: img}
    preds = model.run(None, inputs)[0][0][0]
 
 
    bboxes = []
    for i in range(CLASSES):
        bboxes.append([])
    for pred in preds:
        if pred[4]>CONF_TH:
            bbox = np.zeros(6)
            class_score = np.array(pred[5:])
            cls_id = class_score.argmax()
            x,y,iw,ih = pred[:4]
            x1=x-iw/2
            y1=y-ih/2
            x2=x+iw/2
            y2=y+ih/2
            bbox[:4]=[x1,y1,x2,y2]
            bbox[4]=pred[4]
            bbox[5]=cls_id
            bboxes[cls_id].append(bbox)
 
    results,ones=[],[]
    for bboxes_cl in bboxes:
        if len(bboxes_cl) < 1:
            continue
        o = []
        bboxes_c = torch.from_numpy(np.array(bboxes_cl))
        b = bboxes_c[:,:4]
        s = bboxes_c[:,4]
        keep = torchvision.ops.nms(b,s,NMS_TH)
        for k in keep:
            o.append(bboxes_cl[k])
        oo = np.array(o)
        r=np.zeros_like(oo)
        oo[:,0] = oo[:,0]*IMAGE_SIZE[0]/w
        oo[:,1] = oo[:,1]*IMAGE_SIZE[1]/h
        oo[:,2] = oo[:,2]*IMAGE_SIZE[0]/w
        oo[:,3] = oo[:,3]*IMAGE_SIZE[1]/h
        oo[:,4:] = oo[:,4:]
        r[:,0] = oo[:,0]*width
        r[:,1] = oo[:,1]*height
        r[:,2] = oo[:,2]*width
        r[:,3] = oo[:,3]*height
        r[:,4:] = oo[:,4:]
        r = r.tolist()
        #show
        for *xyxy, conf, cls in r:
            label = '{}|{}'.format(int(cls), '%.2f' % conf)
            plot_one_box(xyxy, img_s, color=[0, 0, 255], label=label)
        
        results.append(r)
        ones.append(oo.tolist())
    return results, ones
 
 
if __name__ == '__main__':
    # input args
    parser = argparse.ArgumentParser()
    #parser.add_argument('--imgfile', default=r"./test/videos", help='Select a img folder witch include \"images\"')
    #parser.add_argument('--imgfile', default=r"/home/coco/yolov5-6.1/test/rgb_test.mp4", help='Select a img folder witch include \"images\"')
    parser.add_argument('--imgfile', default=r"./test/", help='Select a img folder witch include \"images\"')
    parser.add_argument('-d', '--draw', default=True, action="store_true",help='if need draw result image')
    args = parser.parse_args()
 
 
    outoutdir = f'output/'
    if not os.path.exists(outoutdir):
        os.mkdir(outoutdir)
    # clls = int(args.cls)
    json_output = outoutdir + f"result.json"
    total_results = []
 
    # find video
    # images = glob.glob(args.imgfile + "/*")
    videos = sorted(Path(args.imgfile).rglob("**/*.mp4")) + \
        sorted(Path(args.imgfile).rglob("**/*.avi"))
    print(f"The number of all processed files: {len(videos)}")
 
    for v in videos:
        print(str(v))
 
        # read
        cap = cv2.VideoCapture(str(v))
 
        # write
        fps = cap.get(5)
        frame_width = int(cap.get(3))
        frame_height = int(cap.get(4))
        frame_size = (frame_width,frame_height)
        outname = outoutdir + str(v.stem) + '.mp4'
        out = cv2.VideoWriter(outname, cv2.VideoWriter_fourcc(*'mp4v'), fps, frame_size)
 
        while True:
            ret, frame = cap.read()
            if ret:
                results, ones = detect(frame)
                frame_copy = frame.copy()
                if len(results)>0:
                    for res in results:
                        frame_copy = draw(frame_copy , res, "1", "1")
                else:
                    frame_copy = draw(frame_copy , results, "1", "1")
 
                out.write(frame_copy)
            else:
                print("end write")
                break
 
 
 
        cap.release()
        out.release()