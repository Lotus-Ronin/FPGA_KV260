import serial
import time

port = 'COM6'
baudrate = 9600
timeout = 1

def Serial_port():
    try:
        ser = serial.Serial(port=port,baudrate=baudrate,timeout=timeout)
        print(f"串口{port}已打开")

    # data_to_send = " "
        ser.write(data_to_send.encode())
        print(f"发送数据：{data_to_send}")

        time.sleep(2)

        if ser.in_waiting > 0 :
            received_data = ser.read(ser.in_waiting).decode()
            print(f"接收到的数据：{received_data}")
        else:
            print("没有接收到数据")

    except serial.SerialException as e:
        print(f"无法打开串口{port}:{e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print(f"串口{port}已关闭")
