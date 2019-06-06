#include "Array_And_BumpSw.h"

uint8_t ARRAY_Value[8],Bump_Value[6],Bump_Value_Flag[6];  

uint8_t Array_Value;

void DelayUs(uint32_t time)  //1us 48��ָ������
{ 
	while(time--)
	{
		uint32_t j=48;
		while(j--);
	}
}

void DelayMs(uint32_t time)
{
	while(time--)
	{
		DelayUs(1000);
	}
	
}


void ARRAY_Init(void)
{
	  MAP_GPIO_setAsOutputPin(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7); //Array 8��pin��ʼ�� 
	  MAP_GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN3); 		 //ARRAY_LED��ʼ��
}
void ARRAY_Generate(void)
{
	    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN3); //ARRAY_LED��
	
//			MAP_GPIO_setAsOutputPin(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7); //P7����ߵ�ƽ
//		  MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
}
void ARRAY_Read(void)
{   
	  //P7��Ϊ����
//		GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);	
//	
//	  DelayMs(1); //��ʱ1ms
	
//    Array_Value=GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);	
//	
//	
//////    //��ÿ���ŵĵ�λ������ص�һ������
//	if(Array_Value&00000001) ARRAY_Value[0]=1;else ARRAY_Value[0]=0;
//  if(Array_Value&00000010) ARRAY_Value[1]=1;else ARRAY_Value[1]=0;
//	if(Array_Value&00000100) ARRAY_Value[2]=1;else ARRAY_Value[2]=0;
//	if(Array_Value&00001000) ARRAY_Value[3]=1;else ARRAY_Value[3]=0;
//	if(Array_Value&00010000) ARRAY_Value[4]=1;else ARRAY_Value[4]=0;
//	if(Array_Value&00100000) ARRAY_Value[5]=1;else ARRAY_Value[5]=0;
//	if(Array_Value&01000000) ARRAY_Value[6]=1;else ARRAY_Value[6]=0;
//	if(Array_Value&10000000) ARRAY_Value[7]=1;else ARRAY_Value[7]=0;

	//while(1)����ж�ȡ
	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN3); //����ARRAY_LED
	DelayMs(10);
	//����Ϊ����
//	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);	
//	DelayMs(1);
//	Array_Value=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);	



//		ARRAY_Value[0]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN0);
//		ARRAY_Value[1]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN1);
//		ARRAY_Value[2]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN2);
//		ARRAY_Value[3]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN3);
//		ARRAY_Value[4]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN4);
//		ARRAY_Value[5]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN5);
//		ARRAY_Value[6]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN6);
//		ARRAY_Value[7]=MAP_GPIO_getInputPinValue(GPIO_PORT_P7,GPIO_PIN7);
//		MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN3); //�ر�ARRAY_LED
}




void Bump_Sw_Init(void)
{
		 /* BumpSW �жϷ�ʽʹ�� */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    MAP_Interrupt_enableInterrupt(INT_PORT4);
	
}

void PORT4_IRQHandler(void)
{
    uint32_t status=0;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, status);	

		if(status&GPIO_PIN0)   Bump_Value[0]=1;
		if(status&GPIO_PIN2)   Bump_Value[1]=1;
		if(status&GPIO_PIN3)   Bump_Value[2]=1;
		if(status&GPIO_PIN5)   Bump_Value[3]=1;
		if(status&GPIO_PIN6)   Bump_Value[4]=1;
		if(status&GPIO_PIN7)   Bump_Value[5]=1;
}


