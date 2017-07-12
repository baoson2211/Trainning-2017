#include <stdio.h>

#include "one-wire.h"
#include "tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void OneWireHigh(){
  // Set as output
  OW_OUT();
  OW_PIN_OPEN_DRAIN();
  // Set high
  OW_HIGH();
}

void OneWireRelease(){
  // Set as output
  OW_OUT();
  OW_PIN_OPEN_DRAIN();
  // Set low
  OW_LOW();
}

unsigned char OneWireRead(){
  return OW_PIN_VALUE();
}

unsigned int OneWireReset(){
  unsigned int OW;

  OneWireRelease();
  delay_us(479); // 480uS Delay
  OneWireHigh();
  delay_us(69);  // wait 70 uS before reading
  //OneWireRelease(); //debugging
  OW = OneWireRead(); // check for OneWire
  delay_us(479);      // 480uS delay
  OneWireHigh();      // give bus back to OneWire
  return OW;
}


void OneWireWriteBit(unsigned char b){
  if(b){
    OneWireRelease();
    delay_us(5);  // wait 6uS
    OneWireHigh();
    delay_us(63); // wait 64uS
  }
  else{
    OneWireRelease();
    delay_us(59); // wait 60uS
    OneWireHigh();
    delay_us(9); // wait 10uS
  }
}



unsigned char OneWireReadBit() {
  unsigned char out;

  OneWireRelease();
  delay_us(5); // wait 6uS
  OneWireHigh();
  delay_us(8); // wait 9uS
  out = OneWireRead();

  if(out) {
    out = 0x01;
    //GPIO_SetBits(GPIOC, GPIO_Pin_4);
  }
  else {
    out = 0x00;
    //GPIO_ResetBits(GPIOC, GPIO_Pin_4);
  }

  delay_us(54); // wait 55uS
  return out;
}

void OneWireWriteByte(unsigned char b) {
  int i;

  for(i=0; i < 8; i++) {
    OneWireWriteBit(b & 0x01); // send LS bit first
    b = b >> 1;
  }
}

unsigned char OneWireReadByte(void){
  unsigned char out;
  int i;

  out = 0;
  for(i = 0; i < 8; i++){       // read in LS bit first
    out = out >> 1;             // get out ready for next bit
    if(OneWireReadBit() & 0x01) // if its a one
      out = out | 0x80;         // place a 1
  }
  return out;
}
