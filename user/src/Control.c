#include "Control.h"
#include "Array_And_BumpSw.h"

float Real_speed=0,Speed_Target,Speed_Bias,Speed_Integral,Speed_Kp,Speed_Kd,Speed_PWM;
float Turn_Bias,Turn_Last_Bias,Turn_Kp,Turn_Kd,Turn_PWM;

float Array_Bias_Conf_Table[6]={-5,-3,-1,1,3,5};  //����Ҽ�
float Array_Bias=0; //ѭ��ƫ��





void Speed(void)
{
		/*�ٶȿ���*/	
	Speed_Bias=Speed_Target-Real_speed;
	Speed_Integral+=Speed_Bias;
	/*�����޷�*/
	if(Speed_Integral>5000) Speed_Integral=5000;
	else if(Speed_Integral<-5000) Speed_Integral=-5000;
	
	Speed_PWM=Speed_Kp*Speed_Bias+Speed_Integral;
}

void Turn(void)
{
	Turn_Kp=10;Turn_Kd=25;
	 /*ת�����*/	
	Turn_Bias=Array_Bias;
	Turn_PWM=Turn_Kp*Turn_Bias+Turn_Kd*Turn_Last_Bias;
	Turn_Last_Bias=Turn_Bias;
}

