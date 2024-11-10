import cv2
import torch
from queue import Queue
from threading import Thread

model = ...
model.eval()
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model.to(device)


frame_queue = Queue(maxsize=10)

def capture_frames():
	cap = cv2.VideoCapture(0)
	while True:
		ret,frame = cap.read()
		if not ret:
			break;
		if not frame_queue.full():
			frame_queue.put(frame)
			
def process_frames():
	while True:
		if not frame_queue.empty():
			frame = frame_queue.get()
			frame = preprocess_frame(frame)
			frame = frame.to(device)
			with torch.no_grad():
				outputs = model(outputs)

Thread(target=capture_frames).start()
Thread(target=process_frames).start()

			