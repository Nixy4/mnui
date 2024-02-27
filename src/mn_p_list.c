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

//!Public Variable
mn_param_t mn_p_list_param = {
  .puFont = u8g2_font_6x10_tf,
  .lpfnEasingCalc = _easing_calc_OutSine,
  .uFrame = 20,
};

//!Private Function
void mn_p_list_hander_prevItem(mn_p_list_t* page, int style)
{
  #if MN_LOG_TRACE_LIST_INDEV
  MN_LOG_TRACE("prev item");
  #endif

  if(page->uItemIndex>0)
  {
    page->uItemIndex--;
    if(page->uPointerIndex>0)
    {
      //移动指示�?1
      page->uPointerIndex--;
      mn_pointer_moveRelative(&mn_pointer1,0,-page->fItemHeight);
    }
    else
    {
      //移动文本列表
      easing_start_relative(&page->eItemTop,page->fItemHeight);
    }
    //调整指示�?1大小
    mn_pointer_resizeTarget(
      &mn_pointer1,
      u8g2_GetStrWidth(mn_u8g2,page->pxItems[page->uItemIndex].pcText)+page->fPaddingX*2,
      mn_pointer1.eH.fCurr
      );
  }

  #if 0
  mn_pointer_moveTarget(&mn_pointer2,mn_pointer2.eX.fCurr,page->fPointer2Height*page->uItemIndex);
  #else 
  mn_pointer_resizeTarget(
    &mn_pointer2,
    mn_pointer2.eW.fCurr,
    mn_ftoi(page->fPointer2Height)*(page->uItemIndex+1)
    );
  #endif
}

void mn_p_list_handler_nextItem(mn_p_list_t* page, int style)
{
  #if MN_LOG_TRACE_LIST_INDEV
  MN_LOG_TRACE("next item");
  #endif

  if(page->uItemIndex<page->uItemCount-1)
  {
    page->uItemIndex++;
    if(page->uPointerIndex<page->uItemDispCount-1)
    {
      page->uPointerIndex++;
      mn_pointer_moveRelative(&mn_pointer1,0,page->fItemHeight);
    }
    else
    {
      easing_start_relative(&page->eItemTop,-page->fItemHeight);
    }
    //调整指示器大�?
    mn_pointer_resizeTarget(
      &mn_pointer1,
      u8g2_GetStrWidth(mn_u8g2,page->pxItems[page->uItemIndex].pcText)+page->fPaddingX*2,
      mn_pointer1.eH.fCurr
      );
  }

  #if 0
  mn_pointer_moveTarget(&mn_pointer2,mn_pointer2.eX.fCurr,page->fPointer2Height*page->uItemIndex);
  #else 
  mn_pointer_resizeTarget(
    &mn_pointer2,
    mn_pointer2.eW.fCurr,
    mn_ftoi(page->fPointer2Height)*(page->uItemIndex+1)
    );
  #endif

}

void mn_p_list_handler_enter(mn_p_list_t* page, int style)
{
  #if MN_LOG_TRACE_LIST_INDEV
  MN_LOG_TRACE("enter");
  #endif

  mn_item = &page->pxItems[page->uItemIndex];

  switch(mn_item->uType)
  {
    case MN_ITEM_TYPE_PAGE:
    {
      #if MN_LOG_TRACE_LIST_INDEV
      MN_LOG_TRACE("itemType:page");
      #endif

      mn_switch_page(mn_item->xData.xChild);
      break;
    }
    case MN_ITEM_TYPE_SWITCH:
    {
      #if MN_LOG_TRACE_LIST_INDEV
      MN_LOG_TRACE("itemType:switch");
      #endif

      mn_item->xData.bValue = !mn_item->xData.bValue;
      if(mn_item->lpfnFunction)
      {
        mn_item->lpfnFunction(NULL,(int)mn_item->xData.bValue);
      }
      mn_repaint_fromValue();
      break;
    }
    case MN_ITEM_TYPE_VALUEINT:
    {
      #if MN_LOG_TRACE_LIST_INDEV
      MN_LOG_TRACE("itemType:valueInt");
      #endif

      mn_window_open(page);
      break;
    }
    case MN_ITEM_TYPE_VALUEFLOAT:
    {
      #if MN_LOG_TRACE_LIST_INDEV
      MN_LOG_TRACE("itemType:valueFloat");
      #endif

      mn_window_open(page);
      break;
    }
    default:
    {
      #if MN_LOG_TRACE_LIST_INDEV
      MN_LOG_TRACE("itemType:default");
      #endif

      break;
    }
  }
}

void mn_p_list_handler_back(mn_p_list_t* page, int style)
{
  #if MN_LOG_TRACE_LIST_INDEV
  MN_LOG_TRACE("back");
  #endif

  if(page->pxParent)
  {
    mn_switch_page(page->pxParent);
  }
  else
  {
    mn_switch_page(page);
  }
}

__MN_STATIC_INLINE
void mn_p_list_handler_indev(mn_p_list_t* page, int style)
{
  uint8_t key = mn_indev_getBuff();
  if(key)
  {
    switch(key)
    {
      case MN_INDEV_KEY_ENTER: mn_p_list_handler_enter(page,0);break;
      case MN_INDEV_KEY_BACK : mn_p_list_handler_back(page,0);break;
      case MN_INDEV_KEY_UP   : mn_p_list_hander_prevItem(page,0);break;
      case MN_INDEV_KEY_DOWN : mn_p_list_handler_nextItem(page,0);break;
      default:break;
    }
    mn_indev_clearBuff();
  }
}

void mn_p_list_handler_easing(mn_p_list_t* page, int style)
{
  easing_update(&page->eX);
  easing_update(&page->eY);
  easing_update(&page->eItemTop);
  easing_update(&page->eItemOffset);

  mn_repaint_fromEasing(&page->eX);
  mn_repaint_fromEasing(&page->eY);
  mn_repaint_fromEasing(&page->eItemTop);
  mn_repaint_fromEasing(&page->eItemOffset);
}

//!Public Function
mn_handle_t mn_p_list_creatGeneral(
  const char*       title,
  mn_p_list_t*      page,
  uint32_t          memType,
  mn_item_t*        items,
  uint32_t          itemCount,
  mn_param_t*       param
  )
{
  memType &= MN_MEM_TYPE_MASK;

  if(memType==MN_MEM_TYPE_DYNAMI)
  {
    page = (mn_p_list_t*)mn_malloc(sizeof(mn_p_list_t));
    if(page==NULL)
    {
      MN_LOG_ERROR("malloc fail");
      return NULL;
    }
  }
  else if(memType==MN_MEM_TYPE_STATIC)
  {
    if(page==NULL)
    {
      MN_LOG_ERROR("static page is NULL");
      return NULL;
    }
    memset(page,0,sizeof(mn_p_list_t));
  }
  else
  {
    MN_LOG_ERROR("memType error");
    return NULL;
  }
  
  //*初始化mn_page_t部分
  page->uType     = MN_PAGE_TYPE_LIST|memType;
  page->uTypeSize = sizeof(mn_p_list_t);
  page->pcTitle   = title;
  page->pxParam   = param ? param : &mn_p_list_param;
  u8g2_SetFont(mn_u8g2, page->pxParam->puFont);
  page->pxParent  = NULL;
  easing_init(&page->eX,EASING_MODE_DEFAULT,
    page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eY,EASING_MODE_DEFAULT,
    page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);

  //*初始化数据
  page->pxItems       = items;
  page->uItemCount    = itemCount;
  page->uItemIndex    = 0;
  page->uPointerIndex = 0;
  page->uItemDispCount = MN_MAX_LIST_ITEM_COUNT;
  page->fItemHeight    = MN_VER_RES/MN_MAX_LIST_ITEM_COUNT;

  easing_init(&page->eItemTop,EASING_MODE_DEFAULT,
    page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);
  easing_init(&page->eItemOffset,EASING_MODE_DEFAULT,
    page->pxParam->lpfnEasingCalc,page->pxParam->uFrame,0,0);

  page->fFontHeight     = u8g2_GetMaxCharHeight(mn_u8g2);
  page->fPaddingX       = u8g2_GetMaxCharWidth(mn_u8g2);
  page->fPaddingY       = (page->fItemHeight-page->fFontHeight)/2;
  page->fPointer2Height = (float)MN_VER_RES / (float)page->uItemCount;

  //*日志
  #if MN_LOG_LEVEL <= MN_LOG_LEVEL_INFO

  MN_LOG_INFO("Addr[%p]",page);
  MN_LOG_INFO("Name[%s]",page->pcTitle);
  MN_LOG_INFO("Type[%X]",page->uType);
  MN_LOG_INFO("Parent[%p]",page->pxParent);
  MN_LOG_INFO("Param[%p]",page->pxParam);
  MN_LOG_INFO("ItemCount[%d]",page->uItemCount);
  MN_LOG_INFO("ItemDispCount[%d]",page->uItemDispCount);
  MN_LOG_INFO("ItemHeight[%f]",page->fItemHeight);
  MN_LOG_INFO("FontHeight[%f]",page->fFontHeight);
  MN_LOG_INFO("PaddingX[%f]",page->fPaddingX);
  MN_LOG_INFO("PaddingY[%f]",page->fPaddingY);
  MN_LOG_INFO("Pointer2Height[%f]",page->fPointer2Height);

  #endif

  return (mn_handle_t)page;
}

void mn_p_list_appear(mn_p_list_t* page, int style)
{
  #if MN_LOG_TRACE_LIST_RANDER
  MN_LOG_TRACE("page:%s",page->pcTitle);
  #endif

  u8g2_SetFont(mn_u8g2, page->pxParam->puFont);
  mn_pointer_set_param(&mn_pointer1,page->pxParam,1);
  mn_pointer_enable(&mn_pointer1);
  mn_pointer_moveTarget(&mn_pointer1,0,page->uPointerIndex*page->fItemHeight);

  //指示器样式
  switch(style&0x000F)
  {
    case 1://指示器纵向展开
    {
      mn_pointer_resizeAbsolut(
        &mn_pointer1,
        mn_pointer1.eW.fCurr,
        0,
        u8g2_GetStrWidth(mn_u8g2,page->pxItems[page->uItemIndex].pcText)+page->fPaddingX*2,
        page->fItemHeight
        );
    }
    case 2://指示器双向展开
    {
      mn_pointer_resizeAbsolut(
        &mn_pointer1,
        0,
        0,
        u8g2_GetStrWidth(mn_u8g2,page->pxItems[page->uItemIndex].pcText)+page->fPaddingX*2,
        page->fItemHeight
        );
    }
    default://指示器不重新展开
    {
      mn_pointer_resizeTarget(
        &mn_pointer1,
        u8g2_GetStrWidth(mn_u8g2,page->pxItems[page->uItemIndex].pcText)+page->fPaddingX*2,
        page->fItemHeight
        );
      break;
    }
  }

  //*指示器2(索引进度指示)
  mn_pointer_set_param(&mn_pointer2,page->pxParam,0);
  mn_pointer_enable(&mn_pointer2);

  if( (style&0x0010U)>>4 )
  {
    //滑块样式
    mn_pointer_moveTarget(
      &mn_pointer2,
      127-3,
      mn_ftoi(page->fPointer2Height)*page->uItemIndex+1
      );
    mn_pointer_resizeTarget(
      &mn_pointer2,
      4,
      mn_ftoi(page->fPointer2Height)
      );
  }
  else
  {
    //进度条样式
    if( (style&0x0020U)>>5 )
    {
      //进度条重新定位
      mn_pointer_moveTarget(&mn_pointer2,
        MN_HOR_RES-MN_PAGE_LIST_POINTER2_WIDTH,0);
    }
    else
    {
      //进度条不重新定位
      mn_pointer2.eX.fCurr = MN_HOR_RES-MN_PAGE_LIST_POINTER2_WIDTH;
      mn_pointer2.eY.fCurr = 0;
    }
    if( (style&0x0040U)>>6 )
    {
      //进度条不重新展开
      mn_pointer_resizeTarget(
        &mn_pointer2,
        MN_PAGE_LIST_POINTER2_WIDTH,
        mn_ftoi(page->fPointer2Height)*(page->uItemIndex+1)
        );
    }
    else
    {
      //进度条重新展开
      mn_pointer_resizeAbsolut(
        &mn_pointer2,
        MN_PAGE_LIST_POINTER2_WIDTH,
        0,
        MN_PAGE_LIST_POINTER2_WIDTH,
        mn_ftoi(page->fPointer2Height)*(page->uItemIndex+1)
        );  
    }
  }

  //*文本列表
  easing_start_absolute(&page->eY,-page->fItemHeight,0);
  easing_start_absolute(&page->eItemOffset,0,page->fItemHeight);
}

void mn_p_list_disappear(mn_p_list_t* page, int style)
{
  #if MN_LOG_TRACE_LIST_RANDER
  MN_LOG_TRACE("page:%s",page->pcTitle);
  #endif

  switch(style)
  {
    default:mn_disappear_dissolve();
    break;
  }
}

void mn_p_list_handler(mn_p_list_t* page, int style)
{
  if(mn_state==MN_STATE_PAGE_NORMAL)
  {
    mn_p_list_handler_indev(page,0);
  }
  mn_p_list_handler_easing(page,0);
}

void mn_p_list_painter(mn_p_list_t* page, int style)
{
  #if MN_LOG_TRACE_LIST_RANDER
  MN_LOG_TRACE("page:%s",page->pcTitle);
  #endif

  uint8_t value_style_flag;
  switch(style)
  {
    case MN_PAGE_LIST_STYLE_0:value_style_flag=U8G2_BTN_BW0;break;
    case MN_PAGE_LIST_STYLE_1:value_style_flag=U8G2_BTN_BW1;break;
    case MN_PAGE_LIST_STYLE_2:value_style_flag=U8G2_BTN_BW1|U8G2_BTN_SHADOW1;break;
  }

  u8g2_SetFont(mn_u8g2, page->pxParam->puFont);

  float item_x = (page->eX.fCurr+page->fPaddingX-1);
  float item_y;

  for(int i=0;i<page->uItemCount;i++)
  {
    item_y = (
      page->eY.fCurr+
      page->fPaddingY+
      page->fFontHeight+
      page->eItemTop.fCurr+
      i*page->eItemOffset.fCurr-2
      );
    //绘制文本部分
    u8g2_DrawStr(mn_u8g2,mn_ftoi(item_x),mn_ftoi(item_y),page->pxItems[i].pcText);

    //绘制数据部分
    switch(page->pxItems[i].uType)
    {
      case MN_ITEM_TYPE_SWITCH:
      {
        //绘制开关
        u8g2_SetFont(mn_u8g2,u8g2_font_m2icon_9_tf);//开关图标的字体
        //计算开关y坐标
        item_y = (
          page->eY.fCurr+
          (page->fItemHeight-u8g2_GetMaxCharHeight(mn_u8g2))/2+
          u8g2_GetMaxCharHeight(mn_u8g2)+
          page->eItemTop.fCurr+
          i*page->eItemOffset.fCurr-2
        );
        //判断开关状态
        const char* SWITCH;

        if(page->pxItems[i].xData.bValue) 
          SWITCH = "\x4c";
        else 
          SWITCH = "\x4b";

        //绘制开关图标
        u8g2_SetDrawColor(mn_u8g2,1);
        u8g2_DrawUTF8(mn_u8g2,MN_HOR_RES-10-u8g2_GetStrWidth(mn_u8g2,SWITCH),mn_ftoi(item_y),SWITCH);
        //恢复页面字体
        u8g2_SetFont(mn_u8g2,page->pxParam->puFont);
        break;
      }
      case MN_ITEM_TYPE_VALUEINT:
      {
        char valueBuff[10]={0};
        sprintf(valueBuff,"%d",page->pxItems[i].xData.sValue.val);
        //绘制数值
        u8g2_DrawButtonUTF8(
          mn_u8g2,
          MN_HOR_RES-u8g2_GetStrWidth(mn_u8g2,valueBuff)-13,
          mn_ftoi(item_y), 
          value_style_flag,
          u8g2_GetStrWidth(mn_u8g2,valueBuff),
          1,
          1,
          valueBuff
          );
        break;
      }
      case MN_ITEM_TYPE_VALUEFLOAT:
      {
        char valueBuff[10]={0};
        sprintf(valueBuff,"%.2f",page->pxItems[i].xData.fValue.val);
        //绘制数值
        u8g2_DrawButtonUTF8(
          mn_u8g2,
          MN_HOR_RES-u8g2_GetStrWidth(mn_u8g2,valueBuff)-13,
          mn_ftoi(item_y), 
          value_style_flag,
          u8g2_GetStrWidth(mn_u8g2,valueBuff),
          1,
          1,
          valueBuff
          );
        break;
      }
      default:
      {
        break;
      }
    }
  }
}