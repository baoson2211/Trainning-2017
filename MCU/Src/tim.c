/**
  * tim.c
  *
  *  Created on: Apr 28, 2017
  *      Author: b4050n
  */

#include "tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void delay_us(uint16_t us) {
  for(;us > 0; --us) __asm__("nop\n"); // use _asm() with Cosmic
  //do {
  //  us--;
  //  __asm__("nop\n nop\n");
  //} while (us);
  //__asm__("nop\n nop\n");
  //__asm__("ldw X, (0x03, sp)\n lab$: decw X\n tnzw X\n nop\n nop\n jrne lab$\n");
}

void delay_ms(uint16_t ms) {
  /* Init TIMER 2 */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);

  /* Init TIMER 3 prescaler: / (2^5) = /32 */
  TIM2_PrescalerConfig(TIM2_PRESCALER_32, TIM2_PSCRELOADMODE_IMMEDIATE);

  /* HSE --> Auto-Reload value: 8M / 32 = 1/4M, 1/4M / 1k = 250 */
  TIM2_SetAutoreload(250);

  /* Counter value: 2, to compensate the initialization of TIMER */
  TIM2_SetCounter(2);

  /* clear update flag */
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);

  /* Enable Counter */
  TIM2_Cmd(ENABLE);

  while(ms--) {
    while((TIM2->SR1 & 0x01) == 0) ;
    TIM2->SR1 &= (uint8_t)(~0x01);
  }

  /* Disable Counter */
  TIM2_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
}
