
#include "include.h"

		uint8 rx,rx1;	
		int Pause=0;
		uint32_t onTime=40;
		uint32_t offTime=60;
		uint8 tx0[]="state.n0.val=1\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
		uint32_t packetSize0 = sizeof(tx0) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx1[]="state.n1.val=1\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
		uint32_t packetSize1 = sizeof(tx1) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx2[]="state.n2.val=1\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
		uint32_t packetSize2 = sizeof(tx2) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx3[]="state.n3.val=1\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
		uint32_t packetSize3 = sizeof(tx3) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx4[]="state.n3.val=2\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
		uint32_t packetSize4 = sizeof(tx4) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx5[]="speed.n0.val=0\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
		uint32_t packetSize5 = sizeof(tx5) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
	  
		uint8 tx6[]="speed.n0.val=0\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
	  uint32_t packetSize6 = sizeof(tx6) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx7[]="speed.n0.val=30\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
	  uint32_t packetSize7 = sizeof(tx7) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx8[]="speed.n0.val=60\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
	  uint32_t packetSize8 = sizeof(tx8) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		uint8 tx9[]="speed.n0.val=90\xFF\xFF\xFF";						//¶¨ÒåÒª·¢ËÍµÄ×Ö·û´®Êý¾Ý
	  uint32_t packetSize9 = sizeof(tx9) - 1;							//·µ»ØÒª·¢ËÍµÄ×Ö·û´®×Ö·û¸öÊý(¼õÈ¥×Ö·û´®Ä©Î²µÄ¿Õ×Ö·û)
		
		void ControlGPIOWithDelay(uint32_t onTime, uint32_t offTime);
		void USART_HMI(void);

int main(void)
{

	SystemInit(); 			 //ÏµÍ³Ê±ÖÓ³õÊ¼»¯Îª72M	  SYSCLK_FREQ_72MHz
	InitDelay(72);	     //ÑÓÊ±³õÊ¼»¯
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//ÉèÖÃNVICÖÐ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
	InitPWM();
	InitTimer2();				//ÓÃÓÚ²úÉú100usµÄ¶¨Ê±ÖÐ¶Ï
	InitUart1();				//ÓÃÓÚÓëPC¶Ë½øÐÐÍ¨ÐÅ
	InitUart3();				//Íâ½ÓÄ£¿éµÄ´®¿Ú
	InitADC();					//ÊýÄ£×ª»»Ä£¿é
	InitLED();					//LEDµÆ
	InitKey();					//°´¼ü
//	InitBuzzer();				//·äÃùÆ÷
	InitPS2();					//PS2ÓÎÏ·ÊÖ±ú½ÓÊÕÆ÷³õÊ¼»¯
	InitFlash();				//ÉÁ´æ
	InitMemory();				//ÄÚ´æ
	InitBusServoCtrl();	//¶æ»ú¿ØÖÆ
	LED = LED_ON;				//LED´ò¿ª
	
	//¶æ»ú×é¿ØÖÆ
	//BusServoCtrl(1,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(2,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(3,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(4,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(5,SERVO_MOVE_TIME_WRITE,500,1000);
	//BusServoCtrl(6,SERVO_MOVE_TIME_WRITE,500,1000);
	
//		DelayMs(1000);
//µ¥¶æ»ú¿ØÖÆ£¨ÉèÖÃid¡¢Pluse¡¢Time£©
////---ÓÒ
//		ServoSetPluseAndTime( 1, 2100, 1000 );
////---ÖÐ
//DelayMs(1000);
//		ServoSetPluseAndTime( 1, 1500, 1000 );
////---×ó
//DelayMs(1000);
//		ServoSetPluseAndTime( 1, 900, 1000 );

//---×ó¼«ÏÞ
	//ServoSetPluseAndTime( 1, 500, 1000 );
//---ÓÒ¼«ÏÞ	
	//ServoSetPluseAndTime( 1, 2500, 1000 );


	//ServoSetPluseAndTime( 5, 500, 90 );

	while(1)
	{

			USART_HMI();
			ControlGPIOWithDelay(onTime,offTime);
	}
}


void ControlGPIOWithDelay(uint32_t onTime, uint32_t offTime){
				GPIO_SetBits(GPIOC, GPIO_Pin_11); 
        DelayUs(onTime);  
        GPIO_ResetBits(GPIOC, GPIO_Pin_11);  
        DelayUs(offTime);  
}



void USART_HMI(void){
	
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != SET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{ 
    rx = USART_ReceiveData(USART1);
		if (rx == 'A')			
			{	
				LED = LED_OFF;//--ÖÐÖÊ
				ServoSetPluseAndTime( 1, 2500, 90 );
//				DelayMs(100);
				Uart1SendData(rx);
				USART3SendDataPacket(tx2,packetSize2);
			}
		if (rx == 66)			
			{	
				LED = LED_OFF;//--»µ¹û
				ServoSetPluseAndTime( 1, 1800, 90 );
				Uart1SendData(rx);
				USART3SendDataPacket(tx0,packetSize0);
			}
		if (rx == 67)			
				{
				LED = LED_ON;//---µÍÖÊ
				ServoSetPluseAndTime( 1, 1150, 90 );
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
				LED = LED_OFF;//---¸ßÖÊÁ¿
				ServoSetPluseAndTime( 1, 500, 90 );
				Uart1SendData(rx);
				USART3SendDataPacket(tx3,packetSize3);
				Pause=1; 	
				}
		
	}
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != SET){  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
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
