import cv2
import numpy as np
from motion import myOpticalFlow

class App:
    def __init__(self):
        self.frame_idx = 0

        self.fg_alpha = 0.5
        self.curr_filt = np.array([])
        self.kp_list_last = []
        self.des_list_last = []
        self.p1 = []
        self.p2 = []
        self.counter = -1
        self.warp_flags = cv2.INTER_LANCZOS4
        self.diff_factor = 255
        self.flow = myOpticalFlow()
        self.slow = np.array([])
        self.fast = np.array([])
        self.vid_writer = cv2.VideoWriter('track.mp4', cv2.VideoWriter_fourcc(*'mp4v'), 30, (960, 540))

    def run(self, frame):
        M, prev_pts, curr_pts, states = self.flow.update(frame)
        if M is None or self.slow.shape[0] == 0 or self.fast.shape[0] == 0:
            self.slow = frame.copy().astype('float32')
            self.fast = frame.copy().astype('float32')
        else:
            slow_proj = cv2.warpPerspective(self.slow.astype('uint8'), M, (frame.shape[1], frame.shape[0]),
                                            frame.copy(), flags=self.warp_flags, borderMode=cv2.BORDER_TRANSPARENT)
            fast_proj = cv2.warpPerspective(self.fast.astype('uint8'), M, (frame.shape[1], frame.shape[0]),
                                            frame.copy(), flags=self.warp_flags, borderMode=cv2.BORDER_TRANSPARENT)
            self.slow = cv2.addWeighted(slow_proj.astype('float32'), 0.8, frame.astype('float32'), 0.2, 0)
            self.fast = cv2.addWeighted(fast_proj.astype('float32'), 0.3, frame.astype('float32'), 0.7, 0)
        diff = cv2.absdiff(self.slow, self.fast)
        diff_max = np.max(diff)
        diff_factor = 0.95 * self.diff_factor + 0.05 * diff_max
        if diff_factor < diff_max:
            diff_factor = diff_max
        # print("diff_factor:", diff_factor)
        diff_img = (255 * diff.astype('float32') / diff_factor).astype('uint8')
        cv2.imshow("diff", diff_img.astype('uint8'))
        # cv2.imshow("background", self.curr_filt.astype('uint8'))

        frame_feat = frame.astype('uint8').copy()
        for pre, cur in zip(prev_pts, curr_pts):
            pre_x, pre_y = pre[0][0], pre[0][1]
            cur_x, cur_y = cur[0][0], cur[0][1]
            d = ((pre_x - cur_x) ** 2 + (pre_y - cur_y) ** 2) ** 0.5
            if d < 1:
                continue
            cv2.circle(frame_feat, (int(pre[0][0]), int(pre[0][1])), 3, (0, 255, 0), 1, cv2.LINE_AA)
            cv2.circle(frame_feat, (int(cur[0][0]), int(cur[0][1])), 2, (0, 0, 255), 1, cv2.LINE_AA)
            print(d)

        self.vid_writer.write(frame_feat)
        cv2.imshow('features', frame_feat)
        cv2.waitKey(1)


if __name__ == '__main__':
    dc = ['22', '22-2', '26', '35', '36-2', '49', '57', '68', '68-2']
    nq = dc[1]
    # path = r"C:\MyFileAudit\wangman\2023-10\红外视频\焦距{}.mp4".format(nq)
    path = r"F:\dataset\uav\test_video\7.mp4"
    # vid_writer = cv2.VideoWriter('track.mp4', cv2.VideoWriter_fourcc(*'mp4v'), 30, (1920, 1080))
    app = App()
    cam = cv2.VideoCapture(path)
    W, H = 1920 // 2, 1080 // 2
    while True:
        _ret, frame = cam.read()

        if not _ret:
            break

        frame = cv2.resize(frame, (W, H))
        app.run(frame)
        # remained, trs = app.run(frame)
        # cv2.polylines(frame, [np.int32(tr) for tr in trs], False, (0, 255, 0))
        # # draw_str(vis, (20, 20), 'track count: %d' % len(self.tracks))
        # for i, (x1, y1, w1, h1) in enumerate(remained):
        #     cv2.rectangle(frame, (x1, y1), (x1 + w1, y1 + h1), (0, 0, 255), 2)
        # vid_writer.write(frame)
        # cv2.imshow('lk_track', frame)
        # cv2.waitKey(1)

    print('Done')
    cv2.destroyAllWindows()
