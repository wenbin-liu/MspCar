// ------------Motor_Init------------
// Initialize GPIO pins for output, which will be
// used to control the direction of the motors and
// to enable or disable the drivers.
// The motors are initially stopped, the drivers
// are initially powered down, and the PWM speed
// control is uninitialized.
// Input: none
// Output: none
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "motor.h"

int abs(int a )
{  
	if(a>0) return a;
	else    return -a;
}

Timer_A_PWMConfig pwmConfigright=
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        3000,
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_OUTPUTMODE_RESET_SET,
        1500
};
Timer_A_PWMConfig pwmConfigleft=
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        3000,
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_OUTPUTMODE_RESET_SET,
        1500
};

void Motor_Init(void){
  MAP_GPIO_setAsOutputPin(DIR_PORT, DIRR_PIN|DIRL_PIN);// write this as part of Lab 13
  MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(PWM_PORT, PWML_PIN|PWMR_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_GPIO_setAsOutputPin(ENA_PORT, ENAL_PIN|ENAR_PIN);
	MAP_GPIO_setOutputLowOnPin(ENA_PORT, ENAL_PIN|ENAR_PIN);
}

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(void){
  MAP_GPIO_setOutputLowOnPin(ENA_PORT, ENAL_PIN|ENAR_PIN);// write this as part of Lab 13
	pwmConfigright.dutyCycle=0;
	pwmConfigleft.dutyCycle=0;
}

void Motor_Ena(void){
	MAP_GPIO_setOutputHighOnPin(ENA_PORT, ENAL_PIN|ENAR_PIN);
}

void Motor_Changespeed(int motor,int16_t duty)
{
	if(motor==LEFTMOTOR)
	{
		if(duty>0)
		 MAP_GPIO_setOutputLowOnPin(DIR_PORT, DIRL_PIN);
		else
			MAP_GPIO_setOutputHighOnPin(DIR_PORT, DIRL_PIN);
		pwmConfigleft.dutyCycle=abs(duty);
		MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigleft);
	}
	else if(motor==RIGHTMOTOR)
	{
		if(duty>0)
		 MAP_GPIO_setOutputLowOnPin(DIR_PORT, DIRR_PIN);
		else
			MAP_GPIO_setOutputHighOnPin(DIR_PORT, DIRR_PIN);
		pwmConfigright.dutyCycle=abs(duty);
		MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigright);
	}
}
// ------------Motor_Forward------------
// Drive the robot forward by running left and
// right wheels forward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
