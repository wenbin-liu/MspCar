#ifndef _ARRAY_AND_BUMPSW_H
#define _ARRAY_AND_BUMPSW_H

#include "driverlib.h"

extern uint8_t  ARRAY_Value[8],Bump_Value[6],Bump_Value_Flag[6]; 
void DelayUs(uint32_t time);  //1us 48个指令周期
void DelayMs(uint32_t time);
void ARRAY_Init(void);
void ARRAY_Generate(void);
void ARRAY_Read(void);

void Bump_Sw_Init(void);



#endif