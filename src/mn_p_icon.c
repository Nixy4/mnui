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

//!Private Variable
mn_param_t mn_p_icon_param={
  .uFrame = 20,
  .lpfnEasingCalc = _easing_calc_OutCubic,
  .puFont = u8g2_font_6x10_tf,
};

//!Private Function
void mn_p_icon_handler_prevItem(mn_p_icon_t* page)
{

  easing_start_absolute(&page->eTextRise,page->fFontHeight,0);

  if(page->uItemIndex>0)
  {
    page->uItemIndex--;
    easing_start_relative(&page->eItemX,page->fItemInterval);
  }
  else if(page->uItemIndex==0)
  {
    page->uItemIndex=page->uItemCount-1;
    easing_start_relative(&page->eItemX,-page->fItemInterval*(page->uItemCount-1));
  }

}

void mn_p_icon_handler_nextItem(mn_p_icon_t* page)
{
  easing_start_absolute(&page->eTextRise,page->fFontHeight,0);

  if(page->uItemIndex<page->uItemCount-1)
  {
    page->uItemIndex++;
    easing_start_relative(&page->eItemX,-page->fItemInterval);
  }
  else if(page->uItemIndex==page->uItemCount-1)
  {
    page->uItemIndex=0;
    easing_start_relative(&page->eItemX,page->fItemInterval*(page->uItemCount-1));
  }
}

void mn_p_icon_handler_enter(mn_p_icon_t* page)
{
  mn_item = &page->pxItems[page->uItemIndex];

  if(mn_item->lpfnFunction)
  {
    mn_item->lpfnFunction(page,0);
  }
  else
  {
    switch(mn_item->uType)
    {
      case MN_ITEM_TYPE_NONE:
      {
        break;
      }
      case MN_ITEM_TYPE_BUTTON:
      {
        break;
      }
      case MN_ITEM_TYPE_INFO:
      {
        mn_window_open(page);
        break;
      }
      case MN_ITEM_TYPE_PAGE:
      {
        mn_switch_page(mn_item->xData.xChild);
        break;
      }
      case MN_ITEM_TYPE_SWITCH:
      {
        break;
      }
      case MN_ITEM_TYPE_VALUEINT:
      {
        mn_window_open(page);
        break;
      }
      case MN_ITEM_TYPE_VALUEFLOAT:
      {
        break;
      }
    }
  }
}

void mn_p_icon_handler_back(mn_p_icon_t* page)
{
  mn_switch_page(page->pxParent);
}

void mn_p_icon_handler_indev(mn_p_icon_t* page)
{
  uint8_t key = mn_indev_getBuff();
  if(key)
  {
    switch(key)
    {
      case MN_INDEV_KEY_UP   : mn_p_icon_handler_prevItem(page);break;
      case MN_INDEV_KEY_DOWN : mn_p_icon_handler_nextItem(page);break;
      case MN_INDEV_KEY_ENTER: mn_p_icon_handler_enter(page);break;
      case MN_INDEV_KEY_BACK : mn_p_icon_handler_back(page);break;
    }
    mn_indev_clearBuff();
  }
}

void mn_p_icon_handler_easing(mn_p_icon_t* page)
{
  easing_update(&page->eX);
  easing_update(&page->eY);
  easing_update(&page->eItemX);
  easing_update(&page->eItemY);
  easing_update(&page->eItemOffset);
  easing_update(&page->eTextRise);

  mn_repaint_fromEasing(&page->eX);
  mn_repaint_fromEasing(&page->eY);
  mn_repaint_fromEasing(&page->eItemX);
  mn_repaint_fromEasing(&page->eItemY);
  mn_repaint_fromEasing(&page->eItemOffset);
  mn_repaint_fromEasing(&page->eTextRise);
}

//!Public Function
mn_handle_t mn_p_icon_createGeneral(
  const char*         title,
  mn_p_icon_t*        page,
  uint32_t            memType,
  mn_item_t*          items,
  uint16_t            itemCount
  )
{
  //*参数检查
  memType &= MN_MEM_TYPE_MASK;
  //*内存分配
  if(memType==MN_MEM_TYPE_DYNAMI)
  {
    //*动态内存分配
    page = (mn_p_icon_t*)mn_malloc(sizeof(mn_p_icon_t));
    if(page==NULL) return NULL;
  }
  else if(memType==MN_MEM_TYPE_STATIC)
  {
    //*静态内存分配
    if(page==NULL) return NULL;
    else memset(page,0,sizeof(mn_p_icon_t));
  }
  else
  {
    return NULL;
  }
  
  //*初始化mn_page_t部分数据
  page->uType     = MN_PAGE_TYPE_ICON|memType;
  page->uTypeSize = sizeof(mn_p_icon_t);
  page->pcTitle   = title;
  page->pxParam   = &mn_p_icon_param;
  u8g2_SetFont(mn_u8g2, page->pxParam->puFont);
  page->pxParent  = NULL;
  easing_init(&page->eX,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eY,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);

  //*初始化数据
  page->pxItems       = items;
  page->uItemCount    = itemCount;
  page->uItemIndex    = 0;
  page->fItemSide     = 48;
  page->fFontHeight   = u8g2_GetMaxCharHeight(mn_u8g2);
  page->fIconRiseMax  = MN_VER_RES - page->fFontHeight - page->fItemSide;
  page->fIconRiseMax  = (page->fIconRiseMax<0)?0:page->fIconRiseMax;
  page->fItemInterval = page->fItemSide*1.5f;

  easing_init(&page->eItemX,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eItemY,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eItemTop,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eItemOffset,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eTextRise,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eIconRise,
    EASING_MODE_DEFAULT,page->pxParam->lpfnEasingCalc,page->fFontHeight/2,0,0);
  
  //*日志
  #if MN_LOG_LEVEL <= MN_LOG_LEVEL_INFO
  MN_LOG_INFO("Address[%p]",page);
  MN_LOG_INFO("Type:[%X]",page->uType);
  MN_LOG_INFO("TypeSize:[%d]",page->uTypeSize);
  MN_LOG_INFO("Title:[%s]",page->pcTitle);
  MN_LOG_INFO("Param:[%p]",page->pxParam);
  MN_LOG_INFO("Parent:[%p]",page->pxParent);
  MN_LOG_INFO("Items:[%p]",page->pxItems);
  MN_LOG_INFO("ItemCount:[%d]",page->uItemCount);
  MN_LOG_INFO("ItemIndex:[%d]",page->uItemIndex);
  MN_LOG_INFO("ItemSide:[%f]",page->fItemSide);
  MN_LOG_INFO("FontHeight:[%f]",page->fFontHeight);
  MN_LOG_INFO("IconRiseMax:[%f]",page->fIconRiseMax);
  MN_LOG_INFO("ItemInterval:[%f]",page->fItemInterval);
  #endif

  //*返回句柄
  return (mn_handle_t)page;
}

void mn_p_icon_appear(mn_p_icon_t* page,int style)
{
  u8g2_SetFont(mn_u8g2, page->pxParam->puFont);

  mn_pointer_disable(&mn_pointer1);
  mn_pointer_disable(&mn_pointer2);

  easing_start_absolute(&page->eIconRise,page->fIconRiseMax,0);
  easing_start_absolute(&page->eTextRise,page->fFontHeight,0);
  
  switch(style)
  {
    case 1:
    {
      easing_start_absolute(&page->eItemOffset,0,page->fItemInterval);
      easing_start_absolute(
        &page->eItemX,
        -page->fItemSide-MN_HOR_MID,
        -page->fItemSide/2.f-page->fItemInterval*page->uItemIndex
        );
      break;
    }
    default:
    {
      easing_start_absolute(&page->eItemOffset,
        page->fItemInterval,page->fItemInterval);
      easing_start_absolute(&page->eItemY,
        -page->fItemSide,0);
      break;
    }
  }
  //指示器过渡动画启动
  mn_pointer_set_param(&mn_pointer1,page->pxParam,1);
  mn_pointer_enable(&mn_pointer1);
  mn_pointer_moveTarget(&mn_pointer1,MN_HOR_MID-48/2.f,0);
  mn_pointer_resizeTarget(&mn_pointer1,48,48);
}

void mn_p_icon_disappear(mn_p_icon_t* page,int style)
{
  switch(style)
  {
    default:mn_disappear_dissolve();
    break;
  }
}

void mn_p_icon_handler(mn_p_icon_t* page,int style)
{
  //按键信号处理
  if(mn_state==MN_STATE_PAGE_NORMAL)
  {
    //非动画或则窗口状态下,才处理按键
    //窗口状态下要把信号留给窗口的处理函数
    mn_p_icon_handler_indev(page);
  }
  //缓动坐标处理
  mn_p_icon_handler_easing(page);
}

void mn_p_icon_painter(mn_p_icon_t* page,int style)
{
  float x=0;
  float y=0;
  float padding_x=0;
  float padding_y=0;

  switch(style)
  {
    default:
    {
      u8g2_SetDrawColor(mn_u8g2,1);
      for(int i=0; i< page->uItemCount ;i++)
      {
        //*绘制图标部分
        //计算图标边缘填充大小,以使图标居中
        padding_x =  (page->fItemSide - (float)page->pxItems[i].xIcon.uWidth) /2.f;
        padding_y =  (page->fItemSide - (float)page->pxItems[i].xIcon.uHeight) /2.f;

        //当前index图表上升,暂未实际使用
        // if(i==page->uItemIndex)
        // {
        //   y += easing_get_float(&page->eIconRise);
        // }
        x = easing_get_float(&page->eX);
        x += easing_get_float(&page->eItemX);
        // x += easing_get_float(&page->eItemOffset)*i;
        x += page->fItemInterval*i;
        x += padding_x;
        x += MN_HOR_MID;
        x -= page->fItemSide/2.f;
        
        y = easing_get_float(&page->eY);
        y += easing_get_float(&page->eItemY);
        y += padding_y;

        //绘制图标
        u8g2_DrawXBMP(mn_u8g2,
          mn_ftoi(x),mn_ftoi(y),
          page->pxItems[i].xIcon.uWidth,page->pxItems[i].xIcon.uHeight,
          page->pxItems[i].xIcon.pcuData
          );
      }
      //*绘制标题部分
      //坐标计算,居中
      x =  MN_HOR_MID-u8g2_GetStrWidth(mn_u8g2,page->pxItems[page->uItemIndex].pcText)/2+page->eX.fCurr;
      y =  MN_VER_RES+page->eTextRise.fCurr-mn_pointer1.uR*2+page->eY.fCurr;
      //限制绘制区域
      u8g2_SetClipWindow(mn_u8g2,0,50,MN_HOR_RES,MN_VER_RES);
      //绘制标题
      u8g2_DrawStr(
        mn_u8g2,
        mn_ftoi(x),
        mn_ftoi(y),
        page->pxItems[page->uItemIndex].pcText
        );
      //恢复绘制区域
      u8g2_SetMaxClipWindow(mn_u8g2);
      break;
    }
  }
}

