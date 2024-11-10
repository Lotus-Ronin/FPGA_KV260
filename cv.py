import cv2

def open_camera(index=0):
    cap = cv2.VideoCapture(index)
    
    if not cap.isOpened():
        # 释放摄像头资源
        cap.release()
        raise Exception(f"无法打开摄像头，索引: {index}")
    else:
        print("摄像头已成功打开！")
        return cap

try:
    # 尝试打开默认摄像头
    cap = open_camera()
    
    # 如果摄像头成功打开，可以在这里添加代码来读取和显示视频流
    # 例如：
    while True:
        ret, frame = cap.read()
        if not ret:
            print("无法从摄像头读取帧。退出...")
            break
        
        # 显示帧
        cv2.imshow('Camera', frame)
        
        # 按 'q' 退出循环
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    # 释放摄像头资源
    cap.release()
    cv2.destroyAllWindows()

except Exception as e:
    print(e)
