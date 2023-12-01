
import cv2
import numpy as np


class App:
    def __init__(self):
        # 最大轨迹的点数 后面尾巴最多200个点
        self.track_len = 200
        # 每30帧对背景的重新计算并添加到之前的轨迹内
        self.detect_interval = 30
        self.tracks = []

        self.frame_idx = 0
        self.model = cv2.createBackgroundSubtractorMOG2()
        self.kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
        self.lk_params = dict(winSize=(15, 15),
                              maxLevel=2,
                              criteria=(cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))
        self.feature_params = dict(maxCorners=500,
                                   qualityLevel=0.3,
                                   minDistance=7,
                                   blockSize=7)

    def run(self, frame):
        frame_gray = self.model.apply(frame)

        # 腐蚀膨胀 去噪点
        frame_gray = cv2.dilate(frame_gray,self.kernel, 5)
        frame_gray = cv2.erode(frame_gray, self.kernel, 1)
        cv2.imshow('1', frame_gray)
        contours, _ = cv2.findContours(frame_gray, cv2.RETR_EXTERNAL,
                                       cv2.CHAIN_APPROX_SIMPLE)  # 该函数计算一幅图像中目标的轮廓

        frame_gray = cv2.drawContours(frame_gray, contours, -1, (0, 0, 255), 1)
        contours, _ = cv2.findContours(frame_gray, cv2.RETR_EXTERNAL,
                                       cv2.CHAIN_APPROX_SIMPLE)  # 该函数计算一幅图像中目标的轮廓

        # 根据轮廓获取当前的外接矩形
        rect = []
        for c in contours:
            x, y, w, h = cv2.boundingRect(c)
            rect.append([x, y, w, h])

        remained = []
        trs = []
        if len(self.tracks) > 0:
            img0, img1 = self.prev_gray, frame_gray
            p0 = np.float32([tr[-1] for tr in self.tracks]).reshape(-1, 1, 2)
            # 根据前一帧计算当前帧的光流点
            p1, _st, _err = cv2.calcOpticalFlowPyrLK(img0, img1, p0, None, **self.lk_params)
            # 根据当前帧计算前一帧光流点
            p0r, _st, _err = cv2.calcOpticalFlowPyrLK(img1, img0, p1, None, **self.lk_params)
            # 反向光流点的波动如果比较大 就剔除
            d = abs(p0 - p0r).reshape(-1, 2).max(-1)
            good = d < 1
            new_tracks = []
            for tr, (x, y), good_flag in zip(self.tracks, p1.reshape(-1, 2), good):
                if not good_flag:
                    continue
                # 当前帧假如到轨迹内
                tr.append((x, y))
                # 判断当前的光流点是否在检测框内 剔除没有光流目标的检测框
                for i, r in enumerate(rect):
                    if x >= r[0] and x <= (r[0] + r[2]) and y >= r[1] and y <= (r[1] + r[3]):
                        if r not in remained:
                            remained.append(r)

                        index = remained.index(r)
                        if index == len(trs):
                            trs.append([])

                        # 同一个目标有好几个光流关键点轨迹 目前选择最长的一条轨迹作为目标轨迹
                        if len(tr) > len(trs[index]):
                            trs[index] = np.array(tr)
                        break

                if len(tr) > self.track_len:
                    del tr[0]
                new_tracks.append(tr)
            self.tracks = new_tracks

        if self.frame_idx % self.detect_interval == 0:
            # 对光流点进行更新
            mask = np.zeros_like(frame_gray)
            mask[:] = 255
            for x, y in [np.int32(tr[-1]) for tr in self.tracks]:
                cv2.circle(mask, (x, y), 5, 0, -1)
            p = cv2.goodFeaturesToTrack(frame_gray, mask=mask, **self.feature_params)
            if p is not None:
                for x, y in np.float32(p).reshape(-1, 2):
                    self.tracks.append([(x, y)])
        self.prev_gray = frame_gray
        self.frame_idx += 1
        cv2.waitKey(1)
        return remained, trs


if __name__ == '__main__':
    #dc = ['22', '22-2', '26', '35', '36-2', '49', '57', '68', '68-2']
    #nq = dc[1]
    #path = r"C:\MyFileAudit\wangman\2023-10\红外视频\焦距{}.mp4".format(nq)
    path = r"F:\dongmubiao\1.mp4"

    app = App()
    cam = cv2.VideoCapture(path)
    while True:
        _ret, frame = cam.read()

        if not _ret:
            break
        remained, trs = app.run(frame)
        cv2.polylines(frame, [np.int32(tr) for tr in trs], False, (0, 255, 0))
        # draw_str(vis, (20, 20), 'track count: %d' % len(self.tracks))
        for i, (x1, y1, w1, h1) in enumerate(remained):
            cv2.rectangle(frame, (x1, y1), (x1 + w1, y1 + h1), (0, 0, 255), 2)
        cv2.imshow('lk_track', frame)
        cv2.waitKey(1)

    print('Done')
    cv2.destroyAllWindows()
