/*
 * uart.c
 *
 *  Created on: 2019Äê5ÔÂ29ÈÕ
 *      Author: LWB
 */

#include "uart_init.h"
#include "driverlib.h"
#include "stdio.h"
#include <string.h>
//![Simple UART Config]
/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                     // BRDIV = 78
        0xD6,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};
void UART_Init()
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

}

#ifdef __GNUC__
int _write (int fd, char *pBuffer, int size)
  {
      for (int i = 0; i < size; i++)
      {
          MAP_UART_transmitData(EUSCI_A0_BASE, pBuffer[i]);
      }
      return size;
  }
#endif
#ifdef __CC_ARM

int fputc(int c , FILE *fp)
{
    MAP_UART_transmitData(EUSCI_A0_BASE, c);
    return c;

}


#endif
