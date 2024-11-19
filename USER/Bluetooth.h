#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "include.h"


void InitUart3(void);
void TaskBLEMsgHandle(void);
void USART3SendDataPacket(uint8 tx[],uint32 count);

#endif

