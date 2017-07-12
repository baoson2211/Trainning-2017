/**
  * DS18x20.c
  *
  *  Created on: Apr 28, 2017
  *      Author: b4050n
  */

#include "DS18x20.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

long DS18x20_ReadTemp(void) {
  unsigned char LSB;
  unsigned char MSB;

  OneWireReset();                       // Reset Pulse
  OneWireWriteByte(SKIP_ROM);           // Issue skip ROM command (CCh)
  OneWireWriteByte(CONVERT_T);          // Convert T command (44h)
  while(!OW_PIN_VALUE());               // DQ will hold line low while making measurement
  OneWireReset();                       // Start new command sequence
  OneWireWriteByte(SKIP_ROM);           // Issue skip ROM command
  OneWireWriteByte(READ_SCRATCHPAD);    // Read Scratchpad (BEh) - 15 bits
  LSB = OneWireReadByte();
  MSB = OneWireReadByte();
  OneWireReset();                       // Stop Reading
  return (long) ((MSB << 8) | LSB);
}
