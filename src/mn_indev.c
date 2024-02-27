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

#include "mnui.h"
mn_indev_t mn_indev={0};

/**
 * @bref:UI输入设备初始化
 * @param:uType:输入设备驱动类型
 * @param:lpfnGetVal:键值获取函数,仅在轮询模式下有效
 * @param:pvuVal:键值变量指针,仅在中断模式下有效
 * @param:lpfnMapper:键值映射函数
 * @note 当前支持键值: MN_INDEV_KEY_ENTER = 1
 *                    MN_INDEV_KEY_BACK
 *                    MN_INDEV_KEY_UP
 *                    MN_INDEV_KEY_DOWN
 *                    MN_INDEV_KEY_LEFT
 *                    MN_INDEV_KEY_RIGHT
*/
__MN_INLINE void mn_indev_init(
  uint8_t           uType,
  volatile uint8_t* pvuVal,
  mn_indev_callback lpfnGetVal,
  mn_indev_callback lpfnMapper
  )
{
  mn_indev.uType = uType;
  mn_indev.lpfnGetVal = lpfnGetVal;
  mn_indev.pvuVal = pvuVal;
  mn_indev.bEnable = 1;
  mn_indev.lpfnMapper = lpfnMapper;
}

/**
 * @bref:UI输入设备更新
 * @return:void
 * @note:在轮询模式下,每次调用该函数,都会更新键值缓存
 *       在中断模式下,当键值变量不为0时,更新键值缓存
*/
void mn_indev_update()
{
  if(mn_indev.bEnable)
  {
    if(mn_indev.uType==MN_INDEV_TYPE_POLL)
    {
      mn_indev.puValBuf = mn_indev.lpfnMapper(mn_indev.lpfnGetVal(0));
    }
    else if(mn_indev.uType==MN_INDEV_TYPE_IRQ)
    {
      if(*mn_indev.pvuVal!=0)
      {
        mn_indev.puValBuf = mn_indev.lpfnMapper(*mn_indev.pvuVal);
      }
      *mn_indev.pvuVal=0;
    }
  }
}

/**
 * @bref:UI输入设备获取键值缓存
 * @return:键值缓存
*/
__MN_INLINE uint8_t mn_indev_getBuff()
{
  return mn_indev.puValBuf;
}
/**
 * @bref:UI输入设备清空键值缓存
 * @return:void
*/
__MN_INLINE void mn_indev_clearBuff()
{
  mn_indev.puValBuf = 0;
}