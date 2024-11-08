import time
import cv2
import numpy as np
from PIL import Image

from yolo import YOLO


if __name__ == "__main__":
    
    mode = "video"   
    video_path      = 0
    yolo = YOLO()
    
    port = 'COM6'
    baudrate = 9600
    timeout = 1
    
    skip_frames = 9
    frame_count = 0
    
    if mode == "video":
        capture = cv2.VideoCapture(video_path)
        original_fps  = int(capture.get(cv2.CAP_PROP_FPS))
        print(original_fps)
        desired_fps = 24
        if original_fps > desired_fps:
            capture.set(cv2.CAP_PROP_FPS,desired_fps)
        ref, frame = capture.read()
        if not ref:
            raise ValueError("未能正确读取摄像头（视频），请注意是否正确安装摄像头（是否正确填写视频路径）。")

        while(True):
            # 读取某一帧
            ref, frame = capture.read()
            
            if not ref:
                break
            frame_count  +=1
            if frame_count%skip_frames != 0:
                continue    
            frame = cv2.resize(frame,(416,416))
            # 格式转变，BGRtoRGB
            frame = cv2.cvtColor(frame,cv2.COLOR_BGR2RGB)
            # 转变成Image
            frame = Image.fromarray(np.uint8(frame))
            # 进行检测
            frame = np.array(yolo.detect_image(frame))
            # RGBtoBGR满足opencv显示格式
            frame = cv2.cvtColor(frame,cv2.COLOR_RGB2BGR)
            
            cv2.imshow("video",frame)
            c= cv2.waitKey(1) & 0xff
            if c==27:
                capture.release()
                break
        print("Video Detection Done!")
        capture.release()
        cv2.destroyAllWindows()
    else:
        raise AssertionError("Please specify the correct mode: 'predict', 'video', 'fps', 'heatmap', 'export_onnx', 'dir_predict'.")
