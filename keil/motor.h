/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


#ifndef MOTOR_H
#define MOTOR_H

#define DIR_PORT GPIO_PORT_P1
#define DIRR_PIN GPIO_PIN6
#define DIRL_PIN GPIO_PIN7

#define PWM_PORT GPIO_PORT_P2
#define PWML_PIN GPIO_PIN7
#define PWMR_PIN GPIO_PIN6

#define ENA_PORT GPIO_PORT_P3
#define ENAL_PIN GPIO_PIN7
#define ENAR_PIN GPIO_PIN6

#define LEFTMOTOR 0
#define RIGHTMOTOR 1



void Motor_Init(void);
void Motor_Stop(void);
void Motor_Ena(void);
void Motor_Changespeed(int motor,int16_t duty);

#endif


