import cv2
import numpy as np
# def xywh_to_xyxy(boxes):
#     # 将bbox的【x,y,w,h】转换成【x1,y1,x2,y2】
#     # 某些数据集例如 pascal_voc 的标注方式是采用【x，y，w，h】
#     """Convert [x y w h] box format to [x1 y1 x2 y2] format."""
#     return np.hstack((boxes[:, 0:2], boxes[:, 0:2] + boxes[:, 2:4] - 1))
#
#
# def xyxy_to_xywh(boxes):
#     # 上面函数的逆函数
#     """Convert [x1 y1 x2 y2] box format to [x y w h] format."""
#     return np.hstack((boxes[:, 0:2], boxes[:, 2:4] - boxes[:, 0:2] + 1))

def main(path):
    # 第一步：使用cv2.VideoCapture读取视频
    camera = cv2.VideoCapture(path)

    width = int(camera.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(camera.get(cv2.CAP_PROP_FRAME_HEIGHT))

    per_width = int(width / 4)
    per_height = int(height / 2)

    # 第二步：cv2.getStructuringElement构造形态学使用的kernel
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 3))
    # 第三步：构造高斯混合模型
    model = cv2.createBackgroundSubtractorMOG2()
    # model = cv2.createBackgroundSubtractorKNN()

    # 设定区域，后面可以用于判断是否在区域内(可以设置为多边形)
    # contour = np.array([[320, 360], [960, 360], [960, 720], [320, 720]])
    # contour = np.array([[per_width, per_height], [3*per_width, per_height], [3*per_width, height], [per_width, height]])
    contour = np.array([[0, 0], [3 * per_width, per_height], [3 * per_width, height], [0, height]])
    # print(contour)

    # 保存视频时开启
    # videoWrite = cv2.VideoWriter("./result_video/res.mp4", cv2.VideoWriter_fourcc(*"MP4V"), 20, (width, height))

    while True:
        # 第四步：读取视频中的图片，并使用高斯模型进行拟合
        ret, frame = camera.read()
        # 运用高斯模型进行拟合，在两个标准差内设置为0，在两个标准差外设置为255
        fgmk = model.apply(frame)
        # 第五步：使用形态学的开运算做背景的去除
        fgmk = cv2.morphologyEx(fgmk, cv2.MORPH_OPEN, kernel)
        # 第六步：cv2.findContours计算fgmk的轮廓
        contours, hierarchy = cv2.findContours(fgmk.copy(), cv2.RETR_EXTERNAL,
                                               cv2.CHAIN_APPROX_SIMPLE)  # 该函数计算一幅图像中目标的轮廓

        detections = []  # 用于存放boundingbox的起始点坐标、宽、高
        for c in contours:
            # if cv2.contourArea(c) < 500 or cv2.contourArea(c) > 700:
            if cv2.contourArea(c) < 80:
                continue
                # (x, y, w, h) = cv2.boundingRect(c)  # 该函数计算矩形的边界框
                # cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
            else:
                x, y, w, h = cv2.boundingRect(c)  # 该函数计算矩形的边界框
                # print(x, y, w, h)
                # x1, y1, x2, y2 = xywh_to_xyxy([x, y, w, h])
                # y1_offset = int(y1 + ((y2 - y1) * 0.5))
                # x1_offset = int(x1 + ((x2 - x1) * 0.5))
                center = (x, y)

                # 判断点是否在多边形区域内
                result = cv2.pointPolygonTest(contour, center, False)

                if result > 0:
                    cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

        # 第八步：进行图片的展示
        cv2.imshow('fgmk', fgmk)
        cv2.imshow('frame', frame)

        if cv2.waitKey(1) & 0xff == 27:
            break

        # 保存视频时开启
        # videoWrite.write(frame)
    camera.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    path = "./video/test2.mp4"
    main(path)
