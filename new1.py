
import tkinter as tk
from tkinter import filedialog
import cv2
from PIL import Image, ImageTk
from threading import Thread

class FogRemovalVisualizer:
    def __init__(self, master):
        self.master = master
        master.title("去雾可视化展台")
        
        # 设置窗口的宽度和高度
        master.geometry("400x300")

        # 获取窗口的宽度和高度
        window_width = self.master.winfo_screenwidth()
        window_height = self.master.winfo_screenheight()

##################################################################################################
        # 创建视频播放窗口1及其上方的按钮
        self.video_canvas1 = tk.Canvas(master, width=400, height=300)
        self.video_canvas1.pack()
        self.video_canvas1.place(x=1/6*window_width, y=1/3*window_height)
        
        self.button1 = tk.Button(master, text="打开视频1", command=self.open_video1)
        self.button1.pack()
        self.button1.place(x=1/6*window_width+200, y=1/3*window_height-50)#200是画面宽的一半，50是自己设置的
        # 进度条1
        self.progress_bar1 = tk.Canvas(master, height=10, bg="gray")
        
        # 鼠标拖拽
        self.video_canvas1.bind("<B1-Motion>", self.on_drag1) 
        
#####################################################################################################
        # 创建视频播放窗口2及其上方的按钮
        self.video_canvas2 = tk.Canvas(master, width=400, height=300)
        self.video_canvas2.pack()
        self.video_canvas2.place(x=3/6*window_width, y=1/3*window_height)
        
        self.button2 = tk.Button(master, text="打开视频2", command=self.open_video2)
        self.button2.pack()
        self.button2.place(x=3/6*window_width+200, y=1/3*window_height-50)#200是画面宽的一半，50是自己设置的
        # 进度条2
        self.progress_bar2 = tk.Canvas(master, height=10, bg="gray")
#########################################################################################################             
        
        # 用于停止线程的标志
        self.stop_threads = False

    def open_video1(self):
        file_path = filedialog.askopenfilename(title="选择视频1文件", filetypes=[("视频文件", "*.mp4;*.avi")])
        if file_path:
            print(f"打开视频1: {file_path}")
            # 在这里添加处理视频1的逻辑
            Thread(target=self.play_video, args=(self.video_canvas1, file_path), daemon=True).start()

    def open_video2(self):
        file_path = filedialog.askopenfilename(title="选择视频2文件", filetypes=[("视频文件", "*.mp4;*.avi")])
        if file_path:
            print(f"打开视频2: {file_path}")
            # 在这里添加处理视频2的逻辑
            Thread(target=self.play_video, args=(self.video_canvas2, file_path), daemon=True).start()

    def play_video(self, canvas, file_path):
        cap = cv2.VideoCapture(file_path)
        while not self.stop_threads:
            ret, frame = cap.read()
            if not ret:
                break
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            image = Image.fromarray(frame)
            photo = ImageTk.PhotoImage(image=image)
            canvas.config(width=400, height=300)
            canvas.create_image(0, 0, anchor=tk.NW, image=photo)
            canvas.photo = photo
            canvas.update()
            canvas.after(10)

        cap.release()
    def on_drag1(self, event):
        # 鼠标拖拽时更新视频1进度
        progress_width = event.x
        total_width = self.video_canvas1.winfo_width()
        current_frame = int(progress_width / total_width * self.cap1.get(cv2.CAP_PROP_FRAME_COUNT))
        self.cap1.set(cv2.CAP_PROP_POS_FRAMES, current_frame)
    def on_drag2(self, event):
        # 鼠标拖拽时更新视频2进度
        progress_width = event.x
        total_width = self.video_canvas2.winfo_width()
        current_frame = int(progress_width / total_width * self.cap2.get(cv2.CAP_PROP_FRAME_COUNT))
        self.cap2.set(cv2.CAP_PROP_POS_FRAMES, current_frame)
        

if __name__ == "__main__":
    root = tk.Tk()
    app = FogRemovalVisualizer(root)
    root.mainloop()
