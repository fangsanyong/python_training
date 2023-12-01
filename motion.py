import cv2
import numpy as np


class myOpticalFlow():
    def __init__(self):
        self.prev_gray = np.zeros(0)

    def update(self, frame, mask=None):
        # convert to gray scale
        curr_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # prime the pump if needed
        if self.prev_gray.shape[0] == 0:
            self.prev_gray = curr_gray.copy()

        # scale parameters with frame size
        diag = np.linalg.norm(frame.shape[:2])
        maxc = int(diag / 5)
        if maxc < 200: maxc = 200
        mind = int(diag / 30)
        if mind < 30: mind = 30
        bsize = int(diag / 300)
        if bsize < 3: bsize = 3

        prev_pts = cv2.goodFeaturesToTrack(self.prev_gray,
                                           maxCorners=maxc,
                                           qualityLevel=0.01,
                                           minDistance=mind,
                                           blockSize=bsize,
                                           mask=mask)

        # compute the optical flow
        if prev_pts is not None:
            curr_pts, status, err = cv2.calcOpticalFlowPyrLK(self.prev_gray, curr_gray, prev_pts, None)
        else:
            prev_pts = np.zeros(0)
            curr_pts = np.zeros(0)
            status = np.zeros(0)

        self.prev_gray = curr_gray.copy()

        # Sanity check
        if prev_pts.shape != curr_pts.shape:
            prev_pts = curr_pts

        # Filter only valid points
        idx = np.where(status == 1)[0]
        prev_pts = prev_pts[idx]
        curr_pts = curr_pts[idx]

        if curr_pts.shape[0] >= 4:
            tol = 1
            H, status = cv2.findHomography(prev_pts, curr_pts, cv2.LMEDS, tol)
        else:
            H = np.eye(3)

        return H, prev_pts, curr_pts, status