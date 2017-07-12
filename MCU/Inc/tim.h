/**
  * tim.h
  *
  *  Created on: Apr 28, 2017
  *      Author: b4050n
  */

#ifndef __TIM_H
#define __TIM_H

#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_tim2.h"

/* Exported types ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void delay_us( uint16_t us );
void delay_ms( uint16_t ms );

/*
 * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 * when tick = 1, because 0 equels 65535
 *
 * Thanks for Terentiev Oleg
 */
//static inline void delay_cycl( uint16_t __ticks )
//{
//#if defined(__CSMC__)
///* COSMIC */
//  #define T_COUNT(x) (( F_CPU * x / 1000000UL )-3)/3)
//  // ldw X, __ticks ; insert automaticaly
//  _asm("nop\n $N:\n decw X\n jrne $L\n nop\n ", __ticks);
//#elif defined(__SDCC__)
//  #define T_COUNT(x) (( F_CPU * x / 1000000UL )-5)/5)
//  __asm__("nop\n nop\n");
//  do {         // ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
//     __ticks--;//      2c;                 1c;     2c    ; 1/2c
//  } while ( __ticks );
//  __asm__("nop\n");
//#elif defined(__RCST7__)
///* RAISONANCE */
//  #error ToDo for RAISONANCE
//#elif defined(__ICCSTM8__)
///* IAR */
//  #error ToDo for IAR
//#else
// #error Unsupported Compiler!          /* Compiler defines not found */
//#endif
//}
//
//static inline void delay_us( uint16_t __us )
//{
//  delay_cycl( (uint16_t)( T_COUNT(__us) );
//}
//
//static inline void delay_ms( uint16_t __ms )
//{
//  while(__ms--) delay_us( (uint16_t)( 1000 ));
//}

#endif /* __ONE_WIRE_H */
