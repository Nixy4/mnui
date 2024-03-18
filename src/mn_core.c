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

#include "./mnui.h"
//! Public Variable
u8g2_t*        mn_u8g2;
uint32_t       mn_state;
mn_page_t*     mn_pageCurr;
mn_page_t*     mn_pagePrev;

mn_item_t*     mn_item;
mn_page_t*     mn_page;
mn_p_list_t*   mn_plist;
mn_p_icon_t*   mn_picon;
mn_p_custom_t* mn_pcustom;

mn_param_t mn_global_param = {
  .uFrame = 20,
  .lpfnEasingCalc = _easing_calc_OutCubic,
  .puFont = u8g2_font_6x10_tf,
};

//! Private Variable
static uint32_t repaint;

__MN_STATIC_INLINE
void mn_page_appear(mn_page_t* page)
{
  switch(page->uType&MN_PAGE_TYPE_MASK)
  {
    case MN_PAGE_TYPE_CUSTOM:
    {
      mn_pcustom = (mn_p_custom_t*)page;
      if(mn_pcustom->lpfnAppear) mn_pcustom->lpfnAppear(page,0);
      break;
    }
    case MN_PAGE_TYPE_LIST:
    {
      mn_p_list_appear((mn_p_list_t*)page,0);
      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      mn_p_icon_appear((mn_p_icon_t*)page,0);
      break;
    }
    default:
    {
      break;
    }
  } //switch end
}//mn_page_t_appear end

__MN_STATIC_INLINE
void mn_page_diappear(mn_page_t* page)
{
  if(page==NULL) return;

  uint32_t type = page->uType&MN_PAGE_TYPE_MASK;

  switch(type)
  {
    case MN_PAGE_TYPE_CUSTOM:
    {
      mn_pcustom = (mn_p_custom_t*)page;
      if(mn_pcustom->lpfnDisappear) mn_pcustom->lpfnDisappear(page,0);
      break;
    }
    case MN_PAGE_TYPE_LIST:
    {
      mn_p_list_disappear((mn_p_list_t*)page,0);
      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      mn_p_icon_disappear((mn_p_icon_t*)page,0);
      break;
    }
    default:
    {    
      break;
    }
  } //switch end
}//mn_page_t_diappear end

__MN_STATIC_INLINE
void mn_page_handler(mn_page_t* page)
{
  switch(page->uType&MN_PAGE_TYPE_MASK)
  {
    case MN_PAGE_TYPE_CUSTOM:
    {
      mn_pcustom = (mn_p_custom_t*)page;
      if(mn_pcustom->lpfnHandler) mn_pcustom->lpfnHandler(page,0);
      break;
    }
    case MN_PAGE_TYPE_LIST:
    {
      mn_p_list_handler((mn_p_list_t*)page,0);
      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      mn_p_icon_handler((mn_p_icon_t*)page,0);
      break;
    }
    default:
    {
      break;
    }
  }//switch end
}//mn_page_t_handler end

__MN_STATIC_INLINE
void mn_page_painter(mn_page_t* page)
{
  switch(page->uType&MN_PAGE_TYPE_MASK)
  {
    case MN_PAGE_TYPE_CUSTOM:
    {
      mn_pcustom = (mn_p_custom_t*)page;
      if(mn_pcustom->lpfnPainter) mn_pcustom->lpfnPainter(page,0);
      break;
    }
    case MN_PAGE_TYPE_LIST:
    {
      mn_p_list_painter((mn_p_list_t*)page,0);
      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      mn_p_icon_painter((mn_p_icon_t*)page,0);
      break;
    }
    default:
    {
      break;
    }
  }//switch end
}//mn_page_t_painter end

void mn_init(u8g2_t* u8g2, int arg_int)
{
  mn_u8g2 = u8g2;
  u8g2_SetFont(mn_u8g2, mn_global_param.puFont);
  u8g2_SetBitmapMode(mn_u8g2,1);

  mn_pointer_init(&mn_pointer1,&mn_global_param,1);
  mn_pointer_init(&mn_pointer2,&mn_global_param,1);
  // mn_pointer1.eW.fCurr = 2;
  // mn_pointer2.eW.fCurr = 2;
  // mn_pointer1.eH.fCurr = 2;
  // mn_pointer2.eH.fCurr = 2;
  mn_window_init();
  mn_state = MN_STATE_PAGE_NORMAL;

  switch(arg_int)
  {
    case 1:
    {
      //显示进度条
      char progressBuff[20];
      for(int i=10;i<96;i+=2)
      {
        u8g2_ClearBuffer(mn_u8g2);
        sprintf(progressBuff,"%d%%",(int)(i/96.0*100));
        // u8g2_SetFont(mn_u8g2, u8g2_font_9x15_t_symbols);
        u8g2_SetFont(mn_u8g2, u8g2_font_maniac_tf);
        u8g2_DrawStr(mn_u8g2,MN_VER_RES-u8g2_GetStrWidth(mn_u8g2,"MonoUI")/2,32,"MonoUI");

        u8g2_SetFont(mn_u8g2, u8g2_font_twelvedings_t_all);
        u8g2_DrawStr(mn_u8g2,MN_VER_RES-u8g2_GetStrWidth(mn_u8g2,progressBuff)/2,64,progressBuff);

        u8g2_SetDrawColor(mn_u8g2,1);
        u8g2_DrawBox(mn_u8g2,16,40,i,10);
        u8g2_SendBuffer(mn_u8g2);
        mn_delay(1);
      }
      break;
    }
    default:
    {
      break;
    }
  }//switch end
}//mn_init end

__MN_INLINE
void mn_update()
{
  #if MN_USE_FREERTOS
  taskENTER_CRITICAL();
  #endif

  mn_indev_update();

  //! 1.指示器坐标处理
  if(mn_pointer1.bEnable)
  {
    mn_pointer_handler(&mn_pointer1,mn_pageCurr);
  }
  if(mn_pointer2.bEnable) 
  {
    mn_pointer_handler(&mn_pointer2,mn_pageCurr);
  }

  //! 2.页面或窗口坐标处理(互斥)

  if(mn_state>MN_STATE_WINDOW_CLOSED)
  {
    mn_window_handler(mn_pageCurr, 0);
  }

  mn_page_handler(mn_pageCurr);

  if(repaint)
  {
    u8g2_ClearBuffer(mn_u8g2);

    //! 3.绘制页面(总是绘制)
    mn_page_painter(mn_pageCurr);

    //! 4.绘制窗口(打开窗户时绘制在页面上层)
    if(mn_state>MN_STATE_WINDOW_CLOSED)
    {
      mn_window_painter(mn_pageCurr, 0);//窗口在最上层,最后绘制
    }

    //! 5.绘制指示器(总是绘制在最上层)
    if(mn_pointer1.bEnable) 
    {
      mn_pointer_painter(&mn_pointer1,mn_pageCurr);
    }
    if(mn_pointer2.bEnable) 
    {
      mn_pointer_painter(&mn_pointer2,mn_pageCurr);
    }

    u8g2_SendBuffer(mn_u8g2);

    repaint = 0;
  }
  else
  {
    //! 所有动画完成后,处理状态机
    switch(mn_state)
    {
      case MN_STATE_WINDOW_CLOSING: 
      {
        mn_state = MN_STATE_WINDOW_CLOSED;
        mn_indev_enable();
        break;
      }
      case MN_STATE_WINDOW_OPENING: 
      {
        mn_state = MN_STATE_WINDOW_OPENED;
        mn_indev_enable();
        break;
      }
    }
    // MN_LOG_TRACE("mn_state:%d",mn_state);
  }
  #if MN_USE_FREERTOS
  taskEXIT_CRITICAL();
  #endif
}

void mn_switch_page(mn_handle_t page_targ)
{
  #if MN_USE_FREERTOS
  taskENTER_CRITICAL();
  #endif

  if(page_targ==NULL) 
  {
    #if MN_LOG_LEVEL <= MN_LOG_LEVEL_ERROR
    MN_LOG_ERROR("page_targ is NULL");
    #endif
    return;
  }

  repaint = 1;
  mn_pagePrev = mn_pageCurr;
  mn_pageCurr = (mn_page_t*)page_targ;

  if(mn_pagePrev!=NULL) mn_page_diappear(mn_pagePrev);
  mn_page_appear(mn_pageCurr);

  #if MN_USE_FREERTOS
  taskEXIT_CRITICAL();
  #endif
}

__MN_INLINE 
void mn_repaint_fromEasing(easing* e)
{
  repaint |= !(easing_isok(e));
}

__MN_INLINE 
void mn_repaint_fromValue()
{
  repaint |= 1;
}

__MN_INLINE 
void mn_repaint_fromWindow()
{
  repaint |= 1;
}

__MN_INLINE
void mn_set_pageRelationship(mn_handle_t parent,uint8_t itemIndex,mn_handle_t child)
{
  mn_page = (mn_page_t*)parent;
  mn_page->pxItems[itemIndex].xData.xChild = child;
  mn_page = (mn_page_t*)child;
  mn_page->pxParent = parent;
}

void mn_window_open(mn_handle_t handle)
{
  //windows动画启动
  mn_window_appear(handle, 0);
  //更新状态机
  mn_state = MN_STATE_WINDOW_OPENING;
  mn_indev_disable();
  mn_repaint_fromWindow();
}

void mn_window_close(mn_handle_t handle)
{
  //windows动画启动
  mn_window_disappear(handle, 0);
  //更新状态机
  mn_state=MN_STATE_WINDOW_CLOSING;
  mn_indev_disable();
  mn_repaint_fromWindow();
}

__MN_INLINE
uint8_t mn_get_itemType(mn_item_t* item)
{
  return item->uType;
}

__MN_INLINE
uint8_t mn_get_pageType(mn_page_t* page)
{
  return page->uType&MN_PAGE_TYPE_MASK;
}