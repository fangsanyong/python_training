import tkinter as tk
from tkinter import filedialog
from threading import Thread
import cv2
from PIL import Image, ImageTk

class VideoPlayer:
    def __init__(self, master, video_canvas, progress_bar):
        self.master = master
        self.video_canvas = video_canvas
        self.progress_bar = progress_bar

        # # 创建按钮
        # self.open_button = tk.Button(master, text="打开视频", command=self.open_video)
        # self.play_button = tk.Button(master, text="播放", command=self.play)
        # self.pause_button = tk.Button(master, text="暂停", command=self.pause)
        # self.rewind_button = tk.Button(master, text="后退", command=self.rewind)
        # self.forward_button = tk.Button(master, text="前进", command=self.forward)

        # 视频播放对象
        self.cap = None
        self.is_playing = False
        self.thread = None

        # 鼠标拖拽
        self.progress_bar.bind("<B1-Motion>", self.on_drag)

        # # 布局按钮
        # self.open_button.pack(side=tk.LEFT)
        # self.play_button.pack(side=tk.LEFT)
        # self.pause_button.pack(side=tk.LEFT)
        # self.rewind_button.pack(side=tk.LEFT)
        # self.forward_button.pack(side=tk.LEFT)

    def open_video(self):
        file_path = filedialog.askopenfilename(title="选择视频文件", filetypes=[("视频文件", "*.mp4;*.avi")])
        if file_path:
            print(f"打开视频: {file_path}")
            self.play_video(file_path)

    def play_video(self, file_path):
        self.cap = cv2.VideoCapture(file_path)
        self.is_playing = True
        self.update_video()

    def play(self):
        self.is_playing = True
        self.update_video()

    def pause(self):
        self.is_playing = False

    def rewind(self):
        if self.cap is not None:
            current_frame = int(self.cap.get(cv2.CAP_PROP_POS_FRAMES))
            rewind_frame = max(current_frame - 30, 0)
            self.cap.set(cv2.CAP_PROP_POS_FRAMES, rewind_frame)

    def forward(self):
        if self.cap is not None:
            current_frame = int(self.cap.get(cv2.CAP_PROP_POS_FRAMES))
            total_frames = int(self.cap.get(cv2.CAP_PROP_FRAME_COUNT))
            forward_frame = min(current_frame + 30, total_frames - 1)
            self.cap.set(cv2.CAP_PROP_POS_FRAMES, forward_frame)

    def update_video(self):
        if self.cap is not None and self.is_playing:
            ret, frame = self.cap.read()
            if ret:
                frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                image = Image.fromarray(frame)
                photo = ImageTk.PhotoImage(image=image)

                self.video_canvas.create_image(0, 0, anchor=tk.NW, image=photo)
                self.video_canvas.photo = photo

                # 更新进度条
                current_frame = int(self.cap.get(cv2.CAP_PROP_POS_FRAMES))
                total_frames = int(self.cap.get(cv2.CAP_PROP_FRAME_COUNT))
                progress_width = current_frame / total_frames * self.progress_bar.winfo_width()

                self.progress_bar.delete("all")
                self.progress_bar.create_rectangle(0, 0, progress_width, 10, fill="green", outline="green")

                self.master.after(10, self.update_video)
            else:
                self.is_playing = False
                self.cap.release()

    def on_drag(self, event):
        if self.cap is not None:
            progress_width = event.x
            total_width = self.progress_bar.winfo_width()
            current_frame = int(progress_width / total_width * self.cap.get(cv2.CAP_PROP_FRAME_COUNT))
            self.cap.set(cv2.CAP_PROP_POS_FRAMES, current_frame)


if __name__ == "__main__":
    root = tk.Tk()
    root.title("去雾可视化展台")
    # 创建两个视频播放器实例
    window_width = root.winfo_screenwidth()
    window_height = root.winfo_screenheight()
      
      
################################################################1  
    video_canvas1 = tk.Canvas(root, width=400, height=300)
    
    window_width_1=400   ####video_canvas1 = tk.Canvas(root, width=400, height=300)
    y_label=1/3*window_height-50
    shift0=1/6*window_width+1/2*window_width_1+0
    shift1=shift0+60
    shift2=shift1+40
    shift3=shift2+40
    shift4=shift3+40
    
    video_canvas1.pack()
    video_canvas1.place(x=shift0, y=y_label)
    
    
    progress_bar1 = tk.Canvas(root, height=10, bg="gray")
    progress_bar1.pack(fill=tk.X)
    progress_bar1.place(x=shift0, y=y_label+300)
    
    video_player1 = VideoPlayer(root, video_canvas1, progress_bar1)
    ################################1/6=为画面的1/6，200是窗口的大小 
    open_button = tk.Button(root, text="打开视频", command=video_player1.open_video)
    open_button.pack()
    open_button.place(x=shift0, y=y_label-30)
    
    play_button = tk.Button(root, text="播放", command=video_player1.play)
    play_button.pack()
    play_button.place(x=shift1, y=y_label-30)
    
    pause_button = tk.Button(root, text="暂停", command=video_player1.pause)
    pause_button.pack()
    pause_button.place(x=shift2, y=y_label-30)
    
    
    rewind_button = tk.Button(root, text="后退", command=video_player1.rewind)
    rewind_button.pack()
    rewind_button.place(x=shift3, y=y_label-30)
    
    forward_button = tk.Button(root, text="前进", command=video_player1.forward)
    forward_button.pack()
    forward_button.place(x=shift4, y=y_label-30)

###################################################################2
    video_canvas2 = tk.Canvas(root, width=400, height=300)
    ################################1/6=为画面的1/6，200是窗口的大小
    y_label=1/3*window_height-50
    window_width_2=400   ####video_canvas2 = tk.Canvas(root, width=400, height=300)
    shift0=3/6*window_width+1/2*window_width_2+0
    shift1=shift0+60
    shift2=shift1+40
    shift3=shift2+40
    shift4=shift3+40
    
    video_canvas2.pack()
    video_canvas2.place(x=shift0, y=y_label)
    
    
    progress_bar2 = tk.Canvas(root, height=10, bg="gray")
    progress_bar2.pack(fill=tk.X)
    progress_bar2.place(x=shift0, y=y_label+300)
    
    
    
    
    
    video_player2 = VideoPlayer(root, video_canvas2, progress_bar2)
    # 布局两个播放器
    
    
    open_button = tk.Button(root, text="打开视频", command=video_player2.open_video)
    play_button = tk.Button(root, text="播放", command=video_player2.play)
    pause_button = tk.Button(root, text="暂停", command=video_player2.pause)
    rewind_button = tk.Button(root, text="后退", command=video_player2.rewind)
    forward_button = tk.Button(root, text="前进", command=video_player2.forward)
    
    open_button.pack()
    play_button.pack()
    pause_button.pack()
    rewind_button.pack()
    forward_button.pack()
    
    video_canvas2.place(x=shift0, y=y_label)
    open_button.place(x=shift0, y=y_label-30)
    play_button.place(x=shift1, y=y_label-30)
    pause_button.place(x=shift2, y=y_label-30)
    rewind_button.place(x=shift3, y=y_label-30)
    forward_button.place(x=shift4, y=y_label-30)
    

    root.mainloop()
