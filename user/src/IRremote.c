#include "IRremote.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "stdio.h"
#define IR_RX_PORT GPIO_PORT_P5
#define IR_RX_PIN GPIO_PIN7

#define IR_EN_PORT GPIO_PORT_P4
#define IR_EN_PIN GPIO_PIN4

#define NUMBER_TIMER_CAPTURES 34

#define LED1_PORT GPIO_PORT_P1
#define LED1_PIN GPIO_PIN0


#define BIT0_MIN 3000
#define BIT0_MAX 4000
#define BIT1_MIN 6000
#define BIT1_MAX 7000
#define START_MIN 12000


typedef enum
{
    IR_IDLE,
    IR_RECEIVING
} IR_FSMType ;

/* Timer_A Continuous Mode Configuration Parameter */
const Timer_A_ContinuousModeConfig continuousModeConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_16,       // SMCLK/16 = 3MHz
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_SKIP_CLEAR                   // Skup Clear Counter
};

/* Timer_A Capture Mode Configuration Parameter */
const Timer_A_CaptureModeConfig captureModeConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_2,        // CC Register 2
        TIMER_A_CAPTUREMODE_FALLING_EDGE,          // Rising Edge
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,        // CCIxB Input Select
        TIMER_A_CAPTURE_SYNCHRONOUS,              // Synchronized Capture
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,  // Enable interrupt
        TIMER_A_OUTPUTMODE_OUTBITVALUE            // Output bit value
};
void IR_Init()
{
    // Rx --> P5.7
    // EN --> P4.4
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(IR_RX_PORT, IR_RX_PIN,
            GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsOutputPin(IR_EN_PORT,IR_EN_PIN);

    /* Configuring Capture Mode */
    MAP_Timer_A_initCapture(TIMER_A2_BASE, &captureModeConfig);

    /* Configuring Continuous Mode */
    MAP_Timer_A_configureContinuousMode(TIMER_A2_BASE, &continuousModeConfig);

    MAP_Interrupt_enableInterrupt(INT_TA2_N);


    //Enable IR receiver
        MAP_GPIO_setOutputLowOnPin(IR_EN_PORT, IR_EN_PIN);
        /* Starting the Timer_A2 in continuous mode */
    MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);


    MAP_GPIO_setAsOutputPin(LED1_PORT,LED1_PIN);

}

static uint16_t timerAcaptureValues[NUMBER_TIMER_CAPTURES];
uint8_t IR_Code_Recv;
uint8_t IR_Recv_Flag = 0;
void TA2_N_IRQHandler(void)
{
    static uint32_t timerAcapturePointer = 0;
    static uint16_t counterVal,lastCounterVal;
    static uint16_t deltaVal;

    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_2);
    counterVal= MAP_Timer_A_getCaptureCompareCount(TIMER_A2_BASE,
                                                 TIMER_A_CAPTURECOMPARE_REGISTER_2);

    counterVal = MAP_Timer_A_getCaptureCompareCount(TIMER_A2_BASE,
                                             TIMER_A_CAPTURECOMPARE_REGISTER_2);

    if(timerAcapturePointer != 0)
    {
            deltaVal = counterVal-lastCounterVal;
            if(deltaVal > START_MIN)
            {
                timerAcapturePointer = 1;
            }
            timerAcaptureValues[timerAcapturePointer-1] = deltaVal;

    }

    lastCounterVal = counterVal;
    timerAcapturePointer ++ ;


    if (timerAcapturePointer >= NUMBER_TIMER_CAPTURES)
    {

        for(int i = 17;i <25;i++ )
        {
            IR_Code_Recv = IR_Code_Recv << 1;
            if(timerAcaptureValues[i] > 5000)
                IR_Code_Recv |= 0x01;
        }
        timerAcapturePointer = 0;
        IR_Recv_Flag = 1;

      //     printf("%d\n",IR_Code_Recv);

    }


}
