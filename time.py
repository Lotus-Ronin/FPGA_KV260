方法一：
import time
def my_function():
	pass

start_time = time.time()
my_function()
end_time = time.time()
print(f"函数执行耗时：{end_time-start_time}秒")


方法二：
import timeit

def my_function():
	pass
	
execution_time = timeit.timeit(my_function,number=1)
print(f"函数执行耗时：{execution_time}秒")

average_time = timeit.timeit(my_function,number=1000)/1000
print(f"函数执行耗时{average_time}秒")

方法三：
装饰器：
import time
from contextlib imort contexrmanger

@contextmanager
def timeit_context(name):
	start_time = time.time()
	yield
	end_time = time.time()
	print(f"{name}执行耗时:{end_time-start_time}秒")

def my_function():
	pass
	
with timeit_context("my_function")
	my_function()
#with：告诉程序我们想进入一个上下文


---
降低帧率：
import cv2
cap = cv2.VideoCapture(0)

original_fps = int(cap.get(cv2.CAP_PROP_FPS))

desired_fps = 10
if original_fps > desired_fps:
	cap.set(cv2.CAP_PROP_FPS,desired_fps)
	
skip_frames = 5（每隔5帧处理一帧）
frame_count = 0 

while True:
	ret,frame = cap.read()
	if not ret:
		break
	frame_count +=1
	
	if frame_count %skip_frames == 0:
		cv2.imshow("Frame",frame)
		
	if cv2.waitKey(1)&0xFF ==ord('q'):
		break

cap.realese()
cv2.destoryAllWindows()
	
