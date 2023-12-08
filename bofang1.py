import tkinter as tk
from tkinter import filedialog
import cv2
from PIL import Image, ImageTk
from threading import Thread

class FogRemovalVisualizer:
    def __init__(self, master):
        self.master = master
        master.title("去雾可视化展台")

        # 创建按钮
        self.open_file_button = tk.Button(master, text="打开文件", command=self.open_file)
        self.save_file_button = tk.Button(master, text="保存文件", command=self.save_file)
        self.process_before_button = tk.Button(master, text="打开播放处理前视频", command=lambda: self.process_video(before=True))
        self.process_after_button = tk.Button(master, text="打开处理后视频", command=lambda: self.process_video(before=False))

        # 视频播放区域
        # self.video_label = tk.Label(master)
        # self.video_label.pack()
        self.video_canvas = tk.Canvas(master, width=600, height=400)
        #self.video_canvas = tk.Canvas(master, width=1080, height=720)
        self.video_canvas.place(x=100, y=0)
        
        self.video_canvas.pack()    
        
        # 进度条
        self.progress_bar = tk.Canvas(master, height=10, bg="gray")
        #self.progress_bar.pack(fill=tk.X)
        
        # 鼠标拖拽
        self.video_canvas.bind("<B1-Motion>", self.on_drag) 

        # 视频播放对象
        self.cap = None
        self.thread = None  # 用于处理图像更新的线程
        self.is_playing = False

        # 鼠标拖拽
        self.progress_bar.bind("<B1-Motion>", self.on_drag)

        # 使用Key事件来绑定空格键
        master.bind("<Key>", self.handle_key)

        # 按钮布局
        self.open_file_button.pack(side=tk.LEFT)
        self.save_file_button.pack(side=tk.LEFT)
        self.process_before_button.pack(side=tk.LEFT)
        self.process_after_button.pack(side=tk.LEFT)

    def open_file(self):
        file_path = filedialog.askopenfilename(title="选择视频文件", filetypes=[("视频文件", "*.mp4;*.avi")])
        if file_path:
            print(f"打开文件: {file_path}")
            self.play_video(file_path)

    def save_file(self):
        file_path = filedialog.asksaveasfilename(title="保存文件", defaultextension=".mp4", filetypes=[("视频文件", "*.mp4")])
        if file_path:
            print(f"保存文件: {file_path}")

    def process_video(self, before=True):
        if self.cap is not None:
            self.cap.release()  # 释放先前打开的视频

        file_path = filedialog.askopenfilename(title=f"选择{'处理前' if before else '处理后'}视频", filetypes=[("视频文件", "*.mp4;*.avi")])
        if file_path:
            print(f"打开{'处理前' if before else '处理后'}视频: {file_path}")
            self.play_video(file_path)

    def play_video(self, file_path):
        if self.cap is not None:
            self.cap.release()  # 释放先前打开的视频

        self.cap = cv2.VideoCapture(file_path)
        self.is_playing = True
        self.progress_bar.pack(fill=tk.X)
        self.update_video()

    def update_video(self):
        if self.cap is not None and self.is_playing:
            ret, frame = self.cap.read()
            if ret:
                frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                image = Image.fromarray(frame)
                photo = ImageTk.PhotoImage(image=image)
                # self.video_label.config(image=photo)
                # self.video_label.image = photo
                self.video_canvas.create_image(0, 0, anchor=tk.NW, image=photo)
                self.video_canvas.photo = photo



                # 更新进度条
                current_frame = int(self.cap.get(cv2.CAP_PROP_POS_FRAMES))
                total_frames = int(self.cap.get(cv2.CAP_PROP_FRAME_COUNT))
                progress_width = current_frame / total_frames * self.progress_bar.winfo_width()
                
                #progress_width = current_frame / total_frames * self.video_canvas.winfo_width()
                #self.progress_bar.config(width=progress_width)
                
                self.progress_bar.delete("all")
                self.progress_bar.create_rectangle(0, 0, progress_width, 10, fill="green", outline="green")

                self.master.after(10, self.update_video)
            else:
                self.is_playing = False
                self.cap.release()

    def handle_key(self, event):
        # 在这里处理所有键盘事件
        key = event.keysym
        if key == 'space':
            self.pause_video(event)
        elif key == 'Left':
            self.rewind_video(event)
        elif key == 'Right':
            self.forward_video(event)

    def pause_video(self, event):
        # 暂停/继续视频播放
        self.is_playing = not self.is_playing
        if self.is_playing:
            # 启动图像更新的线程
            self.thread = Thread(target=self.update_video)
            self.thread.start()

    def rewind_video(self, event):
        # 后溯视频
        if self.cap is not None:
            current_frame = int(self.cap.get(cv2.CAP_PROP_POS_FRAMES))
            rewind_frame = max(current_frame - 30, 0)  # 后退30帧
            self.cap.set(cv2.CAP_PROP_POS_FRAMES, rewind_frame)

    def forward_video(self, event):
        # 前溯视频
        if self.cap is not None:
            current_frame = int(self.cap.get(cv2.CAP_PROP_POS_FRAMES))
            total_frames = int(self.cap.get(cv2.CAP_PROP_FRAME_COUNT))
            forward_frame = min(current_frame + 30, total_frames - 1)  # 前进30帧
            self.cap.set(cv2.CAP_PROP_POS_FRAMES, forward_frame)

    def on_drag(self, event):
        # 鼠标拖拽时更新视频进度
        progress_width = event.x
        total_width = self.progress_bar.winfo_width()
        current_frame = int(progress_width / total_width * self.cap.get(cv2.CAP_PROP_FRAME_COUNT))
        self.cap.set(cv2.CAP_PROP_POS_FRAMES, current_frame)

if __name__ == "__main__":
    root = tk.Tk()
    app = FogRemovalVisualizer(root)
    root.mainloop()
