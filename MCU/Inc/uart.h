/*
 * uart.h
 *
 *  Created on: Dec 14, 2016
 *      Author: b4050n
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"
#include "stm8s_uart2.h"

/* Exported types ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
#define  BAUDRATE  115200

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void sendUartFn(char *str);
int UART2_config(void);

#endif /* INC_UART_H_ */
