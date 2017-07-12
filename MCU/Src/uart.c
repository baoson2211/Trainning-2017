/*
 * uart.c
 *
 *  Created on: Dec 14, 2016
 *      Author: b4050n
 */
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  uart send a string
  * @param  str: a pointer is pointed to string address
  * @retval None
  */
void sendUartFn(char *str)
{
    while(*str != '\0') {
        UART2_SendData8(*str);
        while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
        str++;
    }
}

/**
  * @brief  uart configuration
  * @param  None
  * @retval 0
  */
int UART2_config(void)
{
    /* Enable UART clock source */
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);

    /* Init peripheral with some parameters:
     *   - Baudrate: 115200
     *   - Frame:         8
     *   - Stop bit:      1
     *   - No parity check
     *   - Disable synchronous mode clock
     *   - Enable TX and RX
     */
    UART2_Init(BAUDRATE, UART2_WORDLENGTH_8D, UART2_STOPBITS_1,
                         UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE,
                         UART2_MODE_TXRX_ENABLE);
    /* Enable UART2 */
    UART2_Cmd(ENABLE);

    return 0;
}
