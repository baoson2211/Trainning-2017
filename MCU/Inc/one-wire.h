/*
 * uart.h
 *
 *  Created on: Apr 28, 2017
 *      Author: b4050n
 */

#ifndef __ONE_WIRE_H
#define __ONE_WIRE_H

#include "stm8s.h"
#include "stm8s_gpio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/

// 1-Wire Bus connect to DS18B20
// PC4: Port C4
#define ONEWIREBUS_PORT   GPIOC
#define ONEWIREBUS_PIN    GPIO_PIN_4

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define OW_OUT()  (ONEWIREBUS_PORT->DDR |= ONEWIREBUS_PIN)
#define OW_IN()   (ONEWIREBUS_PORT->DDR &= (uint8_t)(~(ONEWIREBUS_PIN)))
#define OW_LOW()  (ONEWIREBUS_PORT->ODR &= (uint8_t)(~ONEWIREBUS_PIN))     /* drive 1-wire bus low */
#define OW_HIGH() (ONEWIREBUS_PORT->ODR |= ONEWIREBUS_PIN)                 /* release 1-wire bus */
#define OW_READ() (uint8_t)(ONEWIREBUS_PORT->IDR & ONEWIREBUS_PIN)         /* read 1-wire bus */

#define OW_PIN_PULL_UP()      (ONEWIREBUS_PORT->CR1 |= ONEWIREBUS_PIN)
#define OW_PIN_FLOATING()     (ONEWIREBUS_PORT->CR1 &= (uint8_t)(~(ONEWIREBUS_PIN)))
#define OW_PIN_PUSH_PULL()    (ONEWIREBUS_PORT->CR1 |= ONEWIREBUS_PIN)
#define OW_PIN_OPEN_DRAIN()   (ONEWIREBUS_PORT->CR1 &= (uint8_t)(~(ONEWIREBUS_PIN)))
#define OW_PIN_VALUE()        ((BitStatus)(ONEWIREBUS_PORT->IDR & (uint8_t)ONEWIREBUS_PIN))

/* Exported functions ------------------------------------------------------- */

unsigned int OneWireReset(void);     // Sends a reset pulse to the sensor
void OneWireWriteBit(uint8_t);       // write a single bit to the OneWire
unsigned char OneWireReadBit(void);  // reads a single bit
void OneWireWriteByte(uint8_t);      // writes a byte
unsigned char OneWireReadByte(void); // reads a byte
unsigned char OneWireRead(void);     // reads the current status of the bus
void OneWireHigh(void);              // sets the bus high
void OneWireRelease(void);           // releases the bus

#endif /* __ONE_WIRE_H */
