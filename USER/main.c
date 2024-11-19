
#include "include.h"

		uint8 rx,rx1;	
		int Pause=0;
		int Flag=4;
		uint32_t onTime=40;
		uint32_t offTime=60;
		uint8 tx0[]="state.n0.val=3\xFF\xFF\xFF";						//定义要发送的字符串数据
		uint32_t packetSize0 = sizeof(tx0) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx1[]="state.n1.val=1\xFF\xFF\xFF";						//定义要发送的字符串数据
		uint32_t packetSize1 = sizeof(tx1) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx2[]="state.n2.val=1\xFF\xFF\xFF";						//定义要发送的字符串数据
		uint32_t packetSize2 = sizeof(tx2) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx3[]="state.n3.val=2\xFF\xFF\xFF";						//定义要发送的字符串数据
		uint32_t packetSize3 = sizeof(tx3) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx4[]="state.n3.val=2\xFF\xFF\xFF";						//定义要发送的字符串数据
		uint32_t packetSize4 = sizeof(tx4) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx5[]="speed.n0.val=0\xFF\xFF\xFF";						//定义要发送的字符串数据
		uint32_t packetSize5 = sizeof(tx5) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
	  
		uint8 tx6[]="speed.n0.val=0\xFF\xFF\xFF";						//定义要发送的字符串数据
	  uint32_t packetSize6 = sizeof(tx6) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx7[]="speed.n0.val=30\xFF\xFF\xFF";						//定义要发送的字符串数据
	  uint32_t packetSize7 = sizeof(tx7) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx8[]="speed.n0.val=60\xFF\xFF\xFF";						//定义要发送的字符串数据
	  uint32_t packetSize8 = sizeof(tx8) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		uint8 tx9[]="speed.n0.val=90\xFF\xFF\xFF";						//定义要发送的字符串数据
	  uint32_t packetSize9 = sizeof(tx9) - 1;							//返回要发送的字符串字符个数(减去字符串末尾的空字符)
		
		void ControlGPIOWithDelay(uint32_t onTime, uint32_t offTime);
		void USART_HMI(void);

int main(void)
{

	SystemInit(); 			 //系统时钟初始化为72M	  SYSCLK_FREQ_72MHz
	InitDelay(72);	     //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	InitPWM();
	InitTimer2();				//用于产生100us的定时中断
	InitUart1();				//用于与PC端进行通信
	InitUart3();				//外接模块的串口
	InitADC();					//数模转换模块
	InitLED();					//LED灯
	InitKey();					//按键
//	InitBuzzer();				//蜂鸣器
	InitPS2();					//PS2游戏手柄接收器初始化
	InitFlash();				//闪存
	InitMemory();				//内存
	InitBusServoCtrl();	//舵机控制
	LED = LED_ON;				//LED打开
	
	//舵机组控制
	//BusServoCtrl(1,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(2,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(3,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(4,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(5,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(6,SERVO_MOVE_TIME_WRITE,500,1000);
	
//		DelayMs(1000);
//单舵机控制（设置id、Pluse、Time）
////---右
//		ServoSetPluseAndTime( 1, 2100, 1000 );
////---中
//DelayMs(1000);
//		ServoSetPluseAndTime( 1, 1500, 1000 );
////---左
//DelayMs(1000);
//		ServoSetPluseAndTime( 1, 900, 1000 );

//---左极限
	//ServoSetPluseAndTime( 1, 500, 1000 );
//---右极限	
	//ServoSetPluseAndTime( 1, 2500, 1000 );


	

	while(1)
	{
		switch(Flag){
			case 1 :ServoSetPluseAndTime( 1, 1700, 90 );break;
			case 2 :ServoSetPluseAndTime( 1, 1300, 90 );break;
			case 3 :ServoSetPluseAndTime( 1, 900, 90 );break;
			case 4 :ServoSetPluseAndTime( 1, 500, 90 );break;			
		}
			USART_HMI();
			ControlGPIOWithDelay(onTime,offTime);
			TaskRun();
	}
}


void ControlGPIOWithDelay(uint32_t onTime, uint32_t offTime){
				GPIO_SetBits(GPIOC, GPIO_Pin_11); 
        DelayUs(onTime);  
        GPIO_ResetBits(GPIOC, GPIO_Pin_11);  
        DelayUs(offTime);  
}



void USART_HMI(void){
	
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != SET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{ 
    rx = USART_ReceiveData(USART1);
		if (rx == 'A')			
			{	
				LED = LED_OFF;//--中质
				ServoSetPluseAndTime( 1, 1500, 90 );
				Flag=1;
				
//				DelayMs(100);
				Uart1SendData(rx);
				USART3SendDataPacket(tx2,packetSize2);
			}
		if (rx == 66)			
			{	
				LED = LED_OFF;//--坏果
				ServoSetPluseAndTime( 1, 1300, 90 );
				Flag=2;
				Uart1SendData(rx);
				USART3SendDataPacket(tx0,packetSize0);
			}
		if (rx == 67)			
				{
				LED = LED_ON;//---低质
				ServoSetPluseAndTime( 1, 900, 90 );
				Flag=3;
				Pause=1; 	
				Uart1SendData(rx);
				USART3SendDataPacket(tx1,packetSize1);
				}
		if (rx == 68)			
				{
					if(Pause){
					Uart1SendData(rx);
					USART3SendDataPacket(tx4,packetSize4);
//					ControlGPIOWithDelay(10,90);
					onTime=10;
					offTime=90;
					ControlGPIOWithDelay(onTime,offTime);
					}
				LED = LED_OFF;//---高质量
				ServoSetPluseAndTime( 1, 500, 90 );
				Flag=4;
				Uart1SendData(rx);
				USART3SendDataPacket(tx3,packetSize3);
				}
		
	}
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != SET){  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		rx1 = USART_ReceiveData(USART3);
		if(rx1 == '0'){
		LED = LED_OFF;
		Uart1SendData(rx1);
		USART3SendDataPacket(tx6,packetSize6);
//		ControlGPIOWithDelay(10,90);
		onTime=10;
		offTime=90;
		ControlGPIOWithDelay(onTime,offTime);			
		}
		if(rx1== '1'){
		LED =LED_ON;
		Uart1SendData(rx1);
		USART3SendDataPacket(tx7,packetSize7);
//		ControlGPIOWithDelay(30,70);
		onTime=40;
		offTime=60;
    ControlGPIOWithDelay(onTime,offTime);			
	  }
		if(rx1 == '2'){
		LED = LED_OFF;
		Uart1SendData(rx1);
		USART3SendDataPacket(tx8,packetSize8);
//		ControlGPIOWithDelay(60,40);
		onTime=70;
		offTime=30;
		ControlGPIOWithDelay(onTime,offTime);
		}
		if(rx1 == '3'){
		LED =LED_ON;
		Uart1SendData(rx1);
		USART3SendDataPacket(tx9,packetSize9);
//		ControlGPIOWithDelay(90,10);
		onTime=90;
		offTime=10;
		ControlGPIOWithDelay(onTime,offTime);
	}
		
}
	
}
