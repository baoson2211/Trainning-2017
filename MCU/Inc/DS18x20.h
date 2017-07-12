/**
  * DS18x20.h
  *
  *  Created on: Apr 28, 2017
  *      Author: b4050n
  */

#ifndef __DS18X20_H
#define __DS18X20_H

#include "one-wire.h"

/* Exported types ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/

/* ROM COMMANDS */
#define SEARCH_ROM         (u8)0XF0
#define READ_ROM           (u8)0x33
#define MATCH_ROM          (u8)0x55
#define SKIP_ROM           (u8)0xCC
#define ALARM_SEARCH       (u8)0XEC
/* FUNCTION COMMANDS */
#define CONVERT_T          (u8)0x44
#define WRITE_SCRATCHPAD   (u8)0x4E
#define READ_SCRATCHPAD    (u8)0xBE
#define COPY_SCRATCHPAD    (u8)0X48
#define RECALL_EE          (u8)0XB8
#define READ_POWER_SUPPLY  (u8)0XB4

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

long DS18x20_ReadTemp(void);

#endif /* __DS18X20_H */
