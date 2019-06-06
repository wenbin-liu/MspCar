
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
#include "motor.h"
#include "IRremote.h"
#include "uart_init.h"
#include "Control.h"
#include "Array_And_BumpSw.h"


extern uint8_t IR_Code_Recv;
extern uint8_t IR_Recv_Flag;


uint32_t count=0,Timer_10ms=0;
void SysTick_Callback(void)
{
	Timer_10ms++;
	if(Timer_10ms>10000)  Timer_10ms=0;

	if(count>10000)  count=0;
	
//	Speed(); 
	if  ( ! (Bump_Value[0]||Bump_Value[1]||Bump_Value[2]||Bump_Value[3]||Bump_Value[4]||Bump_Value[5])  )
       Turn();
	
}

uint8_t temp;
uint32_t clk=0;


// 1  3 6 7 12 16 24 48 64 96 128 192
int main(void)
{
    /* Halting the watchdog */
    uint8_t linePatrol = 0;
  MAP_WDT_A_holdTimer();

  MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
  MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
  MAP_PCM_setPowerState(PCM_AM_DCDC_VCORE1);

	
	clk=CS_getMCLK();
	SysTick_enableModule();
	SysTick_setPeriod(480000); //   48 000 000 1s   MCLK is 48000 000   
	SysTick_registerInterrupt(SysTick_Callback);
	SysTick_enableInterrupt();
	
	Motor_Init();
	Motor_Ena();
	Motor_Changespeed(LEFTMOTOR,0);
	Motor_Changespeed(RIGHTMOTOR,0);

	IR_Init();
	UART_Init();
  ARRAY_Init();
	Bump_Sw_Init();

//	int pwmL =1500 ,pwmR = 1500;
int pwmL =0 ,pwmR = 0;

    while (1)
    {	

			if(linePatrol)
			{

			P5->OUT|=0x08;
			P7->DIR=0XFF;
			P7->OUT=0XFF;
			DelayUs(10);
			P7->DIR=0x00;
      
			DelayUs(416);
		
			P5->OUT&=~0X08;
			DelayMs(10);						
			temp=P7->IN;
			
	
		/*根据光电管阵列求取偏差*/
		  if(temp==24)   Array_Bias=0  ;  //直行
      else if(temp>0&&temp<24)    Array_Bias=-24.0/(float)temp;  //右转
			else if(temp>24 &&temp<=192)    Array_Bias=(float)temp/24.0;  //左转
		   

      if (  !(Bump_Value[0]||Bump_Value[1]||Bump_Value[2]||Bump_Value[3]||Bump_Value[4]||Bump_Value[5])) //非障碍
			{
				pwmL=1000+Turn_PWM;
				pwmR=1000-Turn_PWM;
			}

		/*障碍算法*/
	  if(Bump_Value[0]&&Bump_Value_Flag[0]==0)    //左边有障碍，右倒车一段时间
       {  Timer_10ms=0; Bump_Value_Flag[0]=1;
				  pwmL= 0; pwmR=-1200;   
			 }               			 
		if(Bump_Value[2]&&Bump_Value_Flag[2]==0)	//正前方有障碍
			{
					Timer_10ms=0; Bump_Value_Flag[2]=1;
				  pwmL= -400; pwmR=-1200;    
			}  
		if(Bump_Value[3]&&Bump_Value_Flag[3]==0)	//正前方有障碍
			{
					Timer_10ms=0; Bump_Value_Flag[3]=1;
				  pwmL= -1200; pwmR=-400;    
			}  			
		if(Bump_Value[5]&&Bump_Value_Flag[5]==0)  //右边有障碍
			{
					Timer_10ms=0; Bump_Value_Flag[5]=1;
				  pwmL= -1200; pwmR=0;    		
			}   
		if(Timer_10ms>100&&(Bump_Value[0]||Bump_Value[2]||Bump_Value[3]||Bump_Value[5])) //1s后清零 			 
	  {
		   for(int i=0;i<6;i++)  	
          {Bump_Value[i]=0; }//障碍处理结束，标志清零		
				Bump_Value_Flag[0]=0;Bump_Value_Flag[2]=0;Bump_Value_Flag[3]=0;Bump_Value_Flag[5]=0;				
				pwmL=1000; pwmR=1000;   
		}
		
		
							 Motor_Changespeed(LEFTMOTOR,pwmL);
           Motor_Changespeed(RIGHTMOTOR,pwmR);
		
		
	}
/**********************红外遥控**************************/
		
       if(IR_Recv_Flag == 1)
       {			 
           switch(IR_Code_Recv)
           {
           case KEY_CH:    pwmR+=800;
                           pwmL+=800;
                           break;
           case KEY_VOL_P: pwmR-=800;
                           pwmL-=800;
                           break;
           case KEY_PREV: pwmL-=400;
                           pwmR+=400;
                           break;
           case KEY_PLAY: pwmL+=400;
                           pwmR-=400;
                           break;
            case KEY_EQ:
                        linePatrol = ~linePatrol;
                        break;
           default:        pwmL=0;
                           pwmR=0;
           }
           IR_Recv_Flag =0;
           pwmL = pwmL > 3000? 3000:pwmL;
           pwmR = pwmR > 3000? 3000:pwmR;
           pwmL = pwmL < -3000? -3000:pwmL;
           pwmR = pwmR < -3000? -3000:pwmR;
					 
					 Motor_Changespeed(LEFTMOTOR,pwmL);
           Motor_Changespeed(RIGHTMOTOR,pwmR);
       }



        if(pwmL>2500) pwmL=2500;
				else if(pwmL<-2500) pwmL=-2500;
				
				if(pwmR>2500)  pwmR=2500;
				else if(pwmR<-2500) pwmR=-2500;
   

    }
}

