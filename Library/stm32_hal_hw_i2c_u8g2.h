#ifndef __STM32_U8G2__
#define U8G2_STM32_HAL_HW_I2C_H

//HAL库
#include "stm32f1xx_hal.h"
//u8g2库
#include "./u8g2/u8g2.h"

void u8g2_init(u8g2_t* u8g2, I2C_HandleTypeDef* hi2c ,uint8_t addr);

#endif //!__STM32_U8G2__