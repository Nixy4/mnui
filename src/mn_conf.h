/*
The MNUI code is licensed under the terms of the GPL v3 license.
Copyright (C) 2024 Nixy·Gan

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __MN_PORT_H__
#define __MN_PORT_H__

//* U8G2库链接
#define MN_U8G2_INC "u8g2.h"

//* 显示参数配置
#define MN_HOR_RES             128.f
#define MN_VER_RES             64.f
#define MN_MAX_FRAME_COUNT     20
#define MN_MAX_LIST_ITEM_COUNT 4

//* 必要接口
#include "stm32f1xx_hal.h"
#define port_mn_getTick()    HAL_GetTick()

#include "stm32_cmsis_systick_delay.h"
#define port_mn_delay(x)     delay_ms(x)

#include "stm32_hal_hw_uart_debug.h"
#define port_mn_log(fmt,...) freertos_printf(fmt,##__VA_ARGS__)

//* 日志配置
/**
 * MN_LOG_LEVEL_TRACE 
 * MN_LOG_LEVEL_DEBUG
 * MN_LOG_LEVEL_INFO
 * MN_LOG_LEVEL_WARN
 * MN_LOG_LEVEL_ERROR
 * MN_LOG_LEVEL_NONE
*/

#define MN_USE_LOG      0
#define MN_LOG_LEVEL    MN_LOG_LEVEL_TRACE

#define MN_LOG_TRACE_WINDOW_RANDER 1
#define MN_LOG_TRACE_LIST_INDEV 1
#define MN_LOG_TRACE_LIST_RANDER 1


//* 是否使用FreeRTOS
#define MN_USE_FREERTOS 0

//* 内部宏定义
#include MN_U8G2_INC
#define __MN_INLINE        __INLINE
#define __MN_STATIC_INLINE __STATIC_INLINE

#ifndef port_mn_getTick
#define mn_get_tick() 0U
#else
#define mn_get_tick() port_mn_getTick()
#endif

#ifndef port_mn_delay
#define mn_delay(x) while(x--)
#else
#define mn_delay(x) port_mn_delay(x)
#endif

#if MN_USE_LOG
#if MN_LOG_USE_PRINTF
#undef  port_mn_log
#define port_mn_log(fmt,...) printf(fmt,##__VA_ARGS__)
#endif
#ifndef port_mn_log
#define port_mn_log(fmt,...) while(1)
#else
#define mn_log(fmt,...) port_mn_log(fmt,##__VA_ARGS__)
#endif
#else
#define mn_log(fmt,...)
#endif

#if MN_USE_FREERTOS
  #include "FreeRTOS.h"
  #include "task.h"
  #undef  mn_delay
  #define mn_delay(x)  vTaskDelay(x)
  #define mn_malloc(x) pvPortMalloc(x)
  #define mn_free(x)   vPortFree(x)
#else
  #include "stdlib.h"
  #define mn_malloc(x) malloc(x)
  #define mn_free(x)   free(x)
#endif 

#endif