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

/**
 * @bref:溶解淡出效果
*/
void mn_disappear_dissolve(void)
{
  uint16_t interval = 30;
  uint8_t* buf_ptr=u8g2_GetBufferPtr(mn_u8g2);
  uint16_t buf_len=u8g2_GetBufferTileWidth(mn_u8g2)*u8g2_GetBufferTileHeight(mn_u8g2)*8;
  MN_LOG_TRACE("1");
  for (int i=0;i<buf_len;i+=1)
  {
    buf_ptr[i] &= 0x55;//01010101
  }
  u8g2_SendBuffer(mn_u8g2);
  mn_delay(interval);

  for (int i=0;i<buf_len;i+=2)
  {
    buf_ptr[i] &= 0xAA;//1010
  }
  u8g2_SendBuffer(mn_u8g2);
  mn_delay(interval);

  for (int i=0;i<buf_len;i+=1)
  {
    buf_ptr[i] &= 0x00;
  }
  u8g2_SendBuffer(mn_u8g2);
  mn_delay(interval);

  for (int i=0;i<buf_len;i+=2)
  {
    buf_ptr[i] &= 0x00;
  }
  u8g2_SendBuffer(mn_u8g2);
}


/**
 * @bref:溶解淡出效果(局部)
*/
void mn_disappear_dissolve_local(uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
  uint16_t interval = 100;
 
  u8g2_SetDrawColor(mn_u8g2,0);
  for(uint16_t i=0;i<w;i++)
  {
    for(uint16_t j=0;j<h;j++)
    {
      if( i%3==0 && j%3==0 ) u8g2_DrawPixel(mn_u8g2,x+i,y+j);
    }
  }
  u8g2_SendBuffer(mn_u8g2);
  mn_delay(interval);
  for(uint16_t i=0;i<w;i++)
  {
    for(uint16_t j=0;j<h;j++)
    {
      if( i%2==0 && j%2==0 ) u8g2_DrawPixel(mn_u8g2,x+i,y+j);
    }
  }
  u8g2_SendBuffer(mn_u8g2);
  mn_delay(interval);
  for(uint16_t i=0;i<w;i++)
  {
    for(uint16_t j=0;j<h;j++)
    {
      u8g2_DrawPixel(mn_u8g2,x+i,y+j);
    }
  }
  u8g2_SendBuffer(mn_u8g2);
  mn_delay(interval);
}

/**
 * @bref:CRT显示器关闭效果
*/
void mn_disappear_crtClose(void)
{
  uint16_t yTop,yBottom,hBox;
  easing e;

  easing_init(&e,EASING_MODE_DEFAULT,_easing_calc_InCubic,10,0,0);
  easing_start_absolute(&e,2,MN_VER_MID);
  do{
    easing_update(&e);
    hBox    = mn_ftoi(e.fCurr);
    yTop    = mn_ftoi(e.fCurr);
    yBottom = mn_ftoi((float)MN_VER_RES-e.fCurr);
    u8g2_SetDrawColor(mn_u8g2,0);
    u8g2_DrawBox(mn_u8g2,0,0,MN_HOR_RES,hBox);
    u8g2_DrawBox(mn_u8g2,0,yBottom,MN_HOR_RES,hBox);
    u8g2_SetDrawColor(mn_u8g2,1);
    u8g2_DrawHLine(mn_u8g2,0,yTop,MN_HOR_RES);
    u8g2_DrawHLine(mn_u8g2,0,yBottom,MN_HOR_RES);
    u8g2_SendBuffer(mn_u8g2);

  }while( !easing_isok(&e) );

  easing_init(&e,EASING_MODE_DEFAULT,_easing_calc_OutCubic,10,0,0);
  easing_start_absolute(&e,0,MN_HOR_MID);
  do{
    easing_update(&e);
    hBox    = mn_ftoi(e.fCurr);
    yTop    = mn_ftoi(e.fCurr);
    yBottom = mn_ftoi(MN_HOR_RES-e.fCurr)+1;

    u8g2_SetDrawColor(mn_u8g2,0);
    u8g2_DrawBox(mn_u8g2,0,0,hBox,MN_HOR_RES);
    u8g2_DrawBox(mn_u8g2,yBottom,0,hBox,MN_HOR_RES);
    u8g2_SendBuffer(mn_u8g2);
  }while( !easing_isok(&e) );

}

__MN_INLINE 
int mn_ftoi(float v)
{
  int ret = (int)v;
  if( (v - (int)v) > 0.5f ) return ret+1; else return ret;
}

void mn_play_animation(const uint8_t* anim[], uint16_t cnt, uint16_t delay, 
  uint16_t x,uint16_t y,uint16_t w, uint16_t h)
{
  for(uint16_t i=0;i<cnt;i++)
  {
    u8g2_ClearBuffer(mn_u8g2);
    u8g2_DrawXBM(mn_u8g2,x,y,w,h,anim[i]);
    u8g2_SendBuffer(mn_u8g2);
    mn_delay(delay);
  }
}

