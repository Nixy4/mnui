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

mn_pointer_t  mn_pointer1;
mn_pointer_t  mn_pointer2;

void mn_pointer_set_param(mn_pointer_t* p,mn_param_t* pa,uint8_t r)
{
  p->pxParam = pa ? pa : &mn_defaultParam;
  p->eX.lpfnCalc = p->pxParam->lpfnEasingCalc;
  p->eX.uFrameCnt = p->pxParam->uFrame;
  p->eY.lpfnCalc = p->pxParam->lpfnEasingCalc;
  p->eY.uFrameCnt = p->pxParam->uFrame;
  p->eW.lpfnCalc = p->pxParam->lpfnEasingCalc;
  p->eW.uFrameCnt = p->pxParam->uFrame;
  p->eH.lpfnCalc = p->pxParam->lpfnEasingCalc;
  p->eH.uFrameCnt = p->pxParam->uFrame;
  p->uR = r;
}

void mn_pointer_init(mn_pointer_t* p,mn_param_t* pa,uint8_t r)
{
  p->bEnable = 0;
  p->pxParam = pa ? pa : &mn_defaultParam;
  easing_init(&p->eX,EASING_MODE_DEFAULT,p->pxParam->lpfnEasingCalc,p->pxParam->uFrame,0,0);
  easing_init(&p->eY,EASING_MODE_DEFAULT,p->pxParam->lpfnEasingCalc,p->pxParam->uFrame,0,0);
  easing_init(&p->eW,EASING_MODE_DEFAULT,p->pxParam->lpfnEasingCalc,p->pxParam->uFrame,0,0);
  easing_init(&p->eH,EASING_MODE_DEFAULT,p->pxParam->lpfnEasingCalc,p->pxParam->uFrame,0,0);
  p->eW.fCurr = 2;
  p->eH.fCurr = 2;
  p->uR = r;
}

__MN_INLINE void mn_pointer_enable(mn_pointer_t* p)
{
  p->bEnable = 1;
}

__MN_INLINE void mn_pointer_disable(mn_pointer_t* p)
{
  p->bEnable = 0;
}

void mn_pointer_moveAbsolut(mn_pointer_t* p,float x1,float y1,float x2,float y2)
{
  easing_start_absolute(&p->eX,x1,x2);
  easing_start_absolute(&p->eY,y1,y2);
}

void mn_pointer_moveTarget(mn_pointer_t* p,float x,float y)
{
  easing_start_target(&p->eX,x);
  easing_start_target(&p->eY,y);
}

void mn_pointer_moveRelative(mn_pointer_t* p,float x,float y)
{
  easing_start_relative(&p->eX,x);
  easing_start_relative(&p->eY,y);
}

__MN_STATIC_INLINE  
float mn_pointer_resizeCorrection(mn_pointer_t* p,float val)
{
  if( val < p->uR*2 ) return p->uR*2;
  else return val;
}

void mn_pointer_resizeAbsolut(mn_pointer_t* p,float w1,float h1,float w2,float h2)
{
  w1 = mn_pointer_resizeCorrection(p,w1);
  h1 = mn_pointer_resizeCorrection(p,h1);
  w2 = mn_pointer_resizeCorrection(p,w2);
  h2 = mn_pointer_resizeCorrection(p,h2);
  easing_start_absolute(&p->eW,w1,w2);
  easing_start_absolute(&p->eH,h1,h2);
}

void mn_pointer_resizeTarget(mn_pointer_t* p,float w,float h)
{
  w = mn_pointer_resizeCorrection(p,w);
  h = mn_pointer_resizeCorrection(p,h);
  easing_start_target(&p->eW,w);
  easing_start_target(&p->eH,h);
}

void mn_pointer_resizeRelative(mn_pointer_t* p,float w,float h)
{
  w = mn_pointer_resizeCorrection(p,w);
  h = mn_pointer_resizeCorrection(p,h);
  easing_start_relative(&p->eW,w);
  easing_start_relative(&p->eH,h);
}

void mn_pointer_handler(mn_pointer_t* p,mn_page_t* page)
{
  easing_update(&p->eX);
  easing_update(&p->eY);
  easing_update(&p->eW);
  easing_update(&p->eH);
  
  mn_repaint_fromEasing(&p->eX);
  mn_repaint_fromEasing(&p->eY);
  mn_repaint_fromEasing(&p->eW);
  mn_repaint_fromEasing(&p->eH);
}

__MN_STATIC_INLINE void mn_pointer_painter_style1(mn_pointer_t* p)
{
  //方形选择框
  u8g2_SetDrawColor(mn_u8g2,1);
  u8g2_DrawBox(
    mn_u8g2, 
    mn_ftoi(p->eX.fCurr),
    mn_ftoi(p->eY.fCurr),
    mn_ftoi(p->eW.fCurr),
    mn_ftoi(p->eH.fCurr)
    );
}

__MN_STATIC_INLINE 
void mn_pointer_painter_style2(mn_pointer_t* pointer)
{
  //边角点缀选择框
  u8g2_SetDrawColor(mn_u8g2,1);
  u8g2_DrawRFrame(
    mn_u8g2, 
    mn_ftoi(pointer->eX.fCurr),
    mn_ftoi(pointer->eY.fCurr),
    mn_ftoi(pointer->eW.fCurr),
    mn_ftoi(pointer->eH.fCurr),      
    pointer->uR
    );
  u8g2_SetDrawColor(mn_u8g2,2);
  u8g2_DrawRBox(
    mn_u8g2, 
    mn_ftoi(pointer->eX.fCurr),
    mn_ftoi(pointer->eY.fCurr),
    mn_ftoi(pointer->eW.fCurr),
    mn_ftoi(pointer->eH.fCurr),
    pointer->uR
    );
}

void mn_pointer_painter(mn_pointer_t* p,mn_page_t* page)
{
  switch(page->uType&MN_PAGE_TYPE_MASK)
  {
    case MN_PAGE_TYPE_LIST:
    {
      if(p==&mn_pointer1)
      {
        mn_pointer_painter_style2(p);
      }
      else if(p==&mn_pointer2)
      {
        mn_pointer_painter_style1(p);
      }
      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      mn_pointer_painter_style2(p);
      break;
    }
  } //switch
} //fonction