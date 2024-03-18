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

//---------------------------------------------------------------------------
#define plistWindowW      (MN_HOR_RES)
#define plistWindowH      (MN_VER_RES-mn_plist->fItemHeight)
#define plistLineW        2

#define plistProcW        100.f
#define plistProcH        (plistWindowH/3.f)
#define plistProcPaddingX ((MN_HOR_RES-(plistProcW+plistLineW))/2.f)
#define plistProcPaddingY (plistWindowH/2.f-plistProcH/2.f-1)

//---------------------------------------------------------------------------
#define piconWindowW      (MN_HOR_RES/2.f)
#define piconWindowH      (MN_VER_RES)
#define piconLineW        2

#define piconProcAreaX    (0)
#define piconProcAreaY    (0)
#define piconProcAreaW    (piconWindowW/2.f)
#define piconProcAreaH    (piconWindowH)

#define piconProcW        (piconProcAreaW/2.f)
#define piconProcH        (piconProcAreaH*3.f/4.f)

#define piconProcPaddingX ((piconProcAreaW-piconProcW)/2.f)
#define piconProcPaddingY ((piconProcAreaH-piconProcH)/2.f)

//---------------------------------------------------------------------------
#define sMax        ( mn_item->xData.sValue.max )
#define sMin        ( mn_item->xData.sValue.min )
#define sVal        ( mn_item->xData.sValue.val )
#define sBlockCount ( mn_item->xData.sValue.blockCount )

#define sBlockVal   ( ((((float)sMax-(float)sMin)/(float)sBlockCount)) )
#define sProcVal    ( (((float)sVal-(float)sMin)/((float)sMax-(float)sMin)*(float)plistProcW) )

#define sBlockW     ( (float)((sVal-sMin)/(sMax-sMin)*plistProcW) )
#define sProcW      ( (float)((sVal-sMin)/(sMax-sMin)*plistProcW) )

#define fMax          ( mn_item->xData.fValue.max )
#define fMin          ( mn_item->xData.fValue.min )
#define fVal          ( mn_item->xData.fValue.val )
#define fBlockCount   ( mn_item->xData.fValue.blockCount )
#define fBlock        ( (fMax-fMin)/fBlockCount)
#define fProcW        ( (float)((fVal-fMin)/(fMax-fMin)*plistProcW) )

mn_window_t   mn_window={0};

void mn_window_init()
{
  mn_window.pxParam = &mn_global_param;
  easing_init(&mn_window.eX,EASING_MODE_DEFAULT,
    mn_window.pxParam->lpfnEasingCalc,mn_window.pxParam->uFrame,0,0);
  easing_init(&mn_window.eY,EASING_MODE_DEFAULT,
    mn_window.pxParam->lpfnEasingCalc,mn_window.pxParam->uFrame,0,0);
  easing_init(&mn_window.eW,EASING_MODE_DEFAULT,
    mn_window.pxParam->lpfnEasingCalc,mn_window.pxParam->uFrame,0,0);
  easing_init(&mn_window.eH,EASING_MODE_DEFAULT,
    mn_window.pxParam->lpfnEasingCalc,mn_window.pxParam->uFrame,0,0);
  easing_init(&mn_window.eProcVal,EASING_MODE_DEFAULT,
    mn_window.pxParam->lpfnEasingCalc,mn_window.pxParam->uFrame,0,0);
}

void mn_window_appear(mn_handle_t handle, int arg_int)
{
  //获取当前操作页面指针
  mn_page = (mn_page_t*)handle;

  #if MN_LOG_TRACE_WINDOW_RANDER
  MN_LOG_TRACE("start");
  MN_LOG_TRACE("page:%s",mn_page->pcTitle);
  #endif

  //同步窗口与页面的缓动运算函数
  mn_window.eX.lpfnCalc = mn_pageCurr->pxParam->lpfnEasingCalc;
  mn_window.eX.lpfnCalc = mn_pageCurr->pxParam->lpfnEasingCalc;
  mn_window.eX.lpfnCalc = mn_pageCurr->pxParam->lpfnEasingCalc;
  mn_window.eX.lpfnCalc = mn_pageCurr->pxParam->lpfnEasingCalc;
  mn_window.eX.lpfnCalc = mn_pageCurr->pxParam->lpfnEasingCalc;

  switch( mn_get_pageType(mn_page) )
  {
    case MN_PAGE_TYPE_LIST:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("pageType:list");
      MN_LOG_TRACE("page case start");
      #endif

      mn_plist = (mn_p_list_t*)handle;
      //窗口大小 = 128*(64-mn_plist->fItemHeight)
      //展开窗口
      mn_window.eX.fCurr = 0;//不使用
      mn_window.eY.fCurr = 0;//不使用
      mn_window.eW.fCurr = MN_HOR_RES;
      easing_start_absolute(&mn_window.eH,0,MN_VER_RES-mn_plist->fItemHeight-4);
      //偏移页面
      easing_start_target(&mn_plist->eY,-mn_plist->uPointerIndex*mn_plist->fItemHeight);
      //偏移指示器
      mn_pointer_moveRelative(&mn_pointer1,0,-mn_plist->uPointerIndex*mn_plist->fItemHeight);
      mn_pointer_moveTarget(&mn_pointer2,MN_HOR_RES,0);
      //重置指示器大小
      mn_pointer_resizeTarget(&mn_pointer1,MN_HOR_RES,MN_VER_RES);
      //重置进度条
      mn_item = &mn_plist->pxItems[mn_plist->uItemIndex];
      switch( mn_item->uType )
      {
        case MN_ITEM_TYPE_VALUEINT:
        {
          #if MN_LOG_TRACE_WINDOW_RANDER
          MN_LOG_TRACE("itemType:valueInt");
          #endif
          //重置进度条宽度
          easing_start_absolute(&mn_window.eProcVal,0,sProcVal);
          break;
        }
        case MN_ITEM_TYPE_VALUEFLOAT:
        {
          #if MN_LOG_TRACE_WINDOW_RANDER
          MN_LOG_TRACE("itemType:valueFloat");
          #endif
          //重置进度条宽度
          easing_start_absolute(&mn_window.eProcVal,0,fProcW);
          break;
        }
        default:break;
      }
      
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("page case end");
      #endif

      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("pageType:icon");
      MN_LOG_TRACE("page case start");
      #endif

      mn_picon = (mn_p_icon_t*)handle;
      //windows数据初始化
      mn_window.eX.fCurr = 0;
      mn_window.eY.fCurr = 0;
      easing_start_absolute(&mn_window.eW,2,64);
      mn_window.eH.fCurr = 64;
      //偏移页面
      easing_start_target(&mn_picon->eX,
        mn_picon->fItemSide/2+(MN_HOR_MID-mn_picon->fItemSide)/2);
      //偏移指示器1
      mn_pointer_moveRelative(&mn_pointer1,
        (mn_picon->fItemSide/2+(MN_HOR_MID-mn_picon->fItemSide)/2),0);

      mn_item = &mn_picon->pxItems[mn_picon->uItemIndex];
      switch( mn_item->uType )
      {
        case MN_ITEM_TYPE_VALUEINT:
        {
          #if MN_LOG_TRACE_WINDOW_RANDER
          MN_LOG_TRACE("itemType:valueInt");
          #endif
          //重置进度条宽度
          easing_start_absolute(&mn_window.eProcVal,0,sProcVal);
          break;
        }
        case MN_ITEM_TYPE_VALUEFLOAT:
        {
          #if MN_LOG_TRACE_WINDOW_RANDER
          MN_LOG_TRACE("itemType:valueFloat");
          #endif
          //重置进度条宽度
          easing_start_absolute(&mn_window.eProcVal,0,fProcW);
          break;
        }
        default:break;
      }

      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("page case end");
      #endif

      break;
    }
    default:
    {
      break;
    }
  }
}

void mn_window_disappear(mn_handle_t handle, int arg_int)
{
  mn_page = (mn_page_t*)handle;

  #if MN_LOG_TRACE_WINDOW_RANDER
  MN_LOG_TRACE("start");
  MN_LOG_TRACE("page:%s",mn_page->pcTitle);
  #endif

  switch( mn_get_pageType(mn_page) )
  {
    case MN_PAGE_TYPE_LIST:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("pageType:list");
      #endif

      mn_plist = (mn_p_list_t*)handle;
      //压缩窗口
      easing_start_target(&mn_window.eY,64);
      easing_start_target(&mn_window.eH,0);
      //偏移页面
      easing_start_target(&mn_plist->eY,0);
      //偏移指示器
      mn_pointer_moveTarget(&mn_pointer1,0,mn_plist->uPointerIndex*mn_plist->fItemHeight);
      mn_pointer_moveTarget(&mn_pointer2,MN_HOR_RES-MN_PAGE_LIST_POINTER2_WIDTH,0);
      //重置指示器大小
      mn_pointer_resizeTarget(&mn_pointer1,
        u8g2_GetStrWidth(mn_u8g2,mn_item->pcText)+mn_plist->fPaddingX*2,
        mn_plist->fItemHeight
      );
      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("pageType:icon");
      #endif

      mn_picon = (mn_p_icon_t*)handle;
      //压缩
      easing_start_target(&mn_window.eW,0);
      //偏移页面
      easing_start_target(&mn_picon->eX,0);
      //偏移指示器
      mn_pointer_moveRelative(&mn_pointer1,
        -(mn_picon->fItemSide/2+(MN_HOR_MID-mn_picon->fItemSide)/2),0);
      break;
    }
    default:
    {
      break;
    }
  }
}

void mn_window_update_value(mn_handle_t handle, int arg_int)
{
  mn_page = (mn_page_t*)handle;
  mn_item = &mn_page->pxItems[mn_page->uItemIndex];

  switch( mn_get_itemType(mn_item) )
  {
    case MN_ITEM_TYPE_VALUEINT:
    {
      if(arg_int)
      {
        //++
        if(sVal<sMax)
        {
          //更新数值
          sVal+=sBlockVal;
          if(sVal>sMax)
          {
            sVal = sMax;
          }
          //更新进度条缓动
          easing_start_target(&mn_window.eProcVal,sProcVal);
        }
      }
      else
      {
        //--
        if(sVal>sMin)
        {
          //更新数值
          sVal-=sBlockVal;
          if(sVal<sMin)
          {
            sVal = sMin;
          }
          //更新进度条缓动
          easing_start_target(&mn_window.eProcVal,sProcVal);
        }
      }
      break;
    }//case MN_ITEM_TYPE_VALUEINT end
    case MN_ITEM_TYPE_VALUEFLOAT:
    {
      if(arg_int)
      {
        //++
        if(fVal<fMax)
        {
          //更新数值
          fVal+=fBlock;
          if(fVal>fMax)
          {
            fVal = fMax;
          }
          //更新进度条缓动
          easing_start_target(&mn_window.eProcVal,fProcW);
        }
      }
      else
      {
        //--
        if(fVal>fMin)
        {
          //更新数值
          fVal-=fBlock;
          if(fVal<fMin)
          {
            fVal = fMin;
          }
          //更新进度条缓动
          easing_start_target(&mn_window.eProcVal,fProcW);
        }
      }
      break;
    }//case MN_ITEM_TYPE_VALUEFLOAT end
  }
  mn_repaint_fromWindow();
}

void mn_window_handler(mn_handle_t handle, int arg_int)
{
  //easing handler
  easing_update(&mn_window.eX);
  easing_update(&mn_window.eY);
  easing_update(&mn_window.eW);
  easing_update(&mn_window.eH);
  easing_update(&mn_window.eProcVal);
  mn_repaint_fromEasing(&mn_window.eX);
  mn_repaint_fromEasing(&mn_window.eY);
  mn_repaint_fromEasing(&mn_window.eW);
  mn_repaint_fromEasing(&mn_window.eH);
  mn_repaint_fromEasing(&mn_window.eProcVal);

  // bool windowRapaint = false;
  // windowRapaint = !(
  //   easing_isok(&mn_window.eX)&
  //   easing_isok(&mn_window.eY)&
  //   easing_isok(&mn_window.eW)&
  //   easing_isok(&mn_window.eH)&
  //   easing_isok(&mn_window.eProcVal)
  // );

  // if(!windowRapaint)
  // {
  //   if(mn_state==MN_STATE_WINDOW_OPENING)
  //   {
  //     mn_state= MN_STATE_WINDOW_OPENED;
  //   }
  //   else if(mn_state==MN_STATE_WINDOW_CLOSING)
  //   {
  //     mn_state= MN_STATE_WINDOW_CLOSED;
  //   } 
  // }

  //indev key handler
  uint8_t key = mn_indev_getBuff();
  mn_page = (mn_page_t*)handle;
  mn_item = &mn_page->pxItems[mn_page->uItemIndex];
  if(key)
  {
    switch(key)
    {
      case MN_INDEV_KEY_ENTER: 
      {
        break;
      }
      case MN_INDEV_KEY_BACK : 
      {
        mn_window_close(handle);
        break;
      }
      case MN_INDEV_KEY_UP   : 
      {
        mn_window_update_value(handle,1);
        if(mn_item->lpfnFunction)
          mn_item->lpfnFunction(handle,1);
        break;
      }
      case MN_INDEV_KEY_DOWN : 
      {
        mn_window_update_value(handle,0);
        if(mn_item->lpfnFunction)
          mn_item->lpfnFunction(handle,0);
        break;
      }
      default:
      {
        break;
      }
    }
    mn_indev_clearBuff();
  }
}

__MN_STATIC_INLINE
void mn_window_painter_p_lsit(mn_handle_t handle, int arg_int)
{
  mn_plist = (mn_p_list_t*)handle;
  //绘制蒙版
  u8g2_SetDrawColor(mn_u8g2,0);
  u8g2_DrawBox(mn_u8g2,
    0, easing_get_int(&mn_pointer1.eY)+mn_plist->fItemHeight,
    easing_get_int(&mn_pointer1.eW),easing_get_int(&mn_pointer1.eH)-mn_plist->fItemHeight);
  //限制绘制窗口
  u8g2_SetClipWindow(mn_u8g2,
    0, 
    easing_get_int(&mn_pointer1.eY)+mn_plist->fItemHeight,
    easing_get_int(&mn_pointer1.eW), 
    easing_get_int(&mn_pointer1.eY)+easing_get_int(&mn_pointer1.eH)
  );
  u8g2_SetDrawColor(mn_u8g2,1);

  //绘制最大值和最小值
  #define    bntFlag (U8G2_BTN_BW0|U8G2_BTN_SHADOW0)
  char       valueBuff[20]  = {0};

  mn_item = &mn_plist->pxItems[mn_plist->uItemIndex];

  switch( mn_get_itemType(mn_item) )
  {
    case MN_ITEM_TYPE_INFO:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("itemType:info");
      #endif
      break;
    }
    case MN_ITEM_TYPE_VALUEINT:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("itemType:valueInt");
      #endif
      //绘制进度条
      u8g2_DrawBox(mn_u8g2,
        plistProcPaddingX,
        mn_plist->fItemHeight+plistProcPaddingY-5,
        easing_get_int(&mn_window.eProcVal),
        plistProcH
      );
      //外侧框
      u8g2_DrawFrame(mn_u8g2,
        plistProcPaddingX-plistLineW,
        mn_plist->fItemHeight+plistProcPaddingY-plistLineW-5,
        plistProcW+plistLineW*2,
        plistProcH+plistLineW*2
      );
      //内侧框
      u8g2_DrawFrame(mn_u8g2,
        plistProcPaddingX-plistLineW/2,
        mn_plist->fItemHeight+plistProcPaddingY-plistLineW/2-5,
        plistProcW+plistLineW,
        plistProcH+plistLineW
      );
      //最小值
      sprintf(valueBuff,"%d",mn_item->xData.sValue.min);
      u8g2_DrawButtonUTF8(mn_u8g2,
        plistProcPaddingX,
        MN_VER_RES-5, 
        bntFlag,u8g2_GetStrWidth(mn_u8g2,valueBuff),0,0,valueBuff
        );
      //最大值
      sprintf(valueBuff,"%d",mn_item->xData.sValue.max);
      u8g2_DrawButtonUTF8(mn_u8g2,
        MN_HOR_RES-plistProcPaddingX-u8g2_GetStrWidth(mn_u8g2,valueBuff),
        MN_VER_RES-5,
        bntFlag,u8g2_GetStrWidth(mn_u8g2,valueBuff),0,0,valueBuff);
      //当前值进度条
      u8g2_DrawBox(mn_u8g2,
        plistProcPaddingX,
        mn_plist->fItemHeight+plistProcPaddingY-5,
        easing_get_int(&mn_window.eProcVal),
        plistProcH
      );
      break;
    }
    case MN_ITEM_TYPE_VALUEFLOAT:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("itemType:valueFloat");
      #endif

      //最小值
      sprintf(valueBuff,"%.2f",mn_item->xData.fValue.min);
      u8g2_DrawButtonUTF8(mn_u8g2,
        plistProcPaddingX,
        MN_VER_RES-5, 
        bntFlag,u8g2_GetStrWidth(mn_u8g2,valueBuff),0,0,valueBuff);
      //最大值
      sprintf(valueBuff,"%.2f",mn_item->xData.fValue.max);
      u8g2_DrawButtonUTF8(mn_u8g2,
        MN_HOR_RES-plistProcPaddingX-u8g2_GetStrWidth(mn_u8g2,valueBuff),
        MN_VER_RES-5,
        bntFlag,u8g2_GetStrWidth(mn_u8g2,valueBuff),0,0,valueBuff);
      //当前值进度条
      u8g2_DrawBox(mn_u8g2,
        plistProcPaddingX,
        mn_plist->fItemHeight+plistProcPaddingY-mn_plist->fPaddingY,
        easing_get_int(&mn_window.eProcVal),
        plistProcH
      );
      break;
    }
    default:break;
  }
  //恢复绘制窗口
  u8g2_SetMaxClipWindow(mn_u8g2);
}

__MN_STATIC_INLINE
void mn_window_painter_p_icon(mn_handle_t handle, int arg_int)
{
  mn_picon = (mn_p_icon_t*)handle;
  // // 限制u8g2绘制窗口
  // u8g2_SetClipWindow(mn_u8g2,
  //   easing_get_int(&mn_window.eX),
  //   easing_get_int(&mn_window.eY),
  //   easing_get_int(&mn_window.eX)+easing_get_int(&mn_window.eW),
  //   easing_get_int(&mn_window.eY)+easing_get_int(&mn_window.eH)
  // );
  //绘制窗口
  u8g2_SetDrawColor(mn_u8g2,0);
  u8g2_DrawBox(mn_u8g2,
    easing_get_int(&mn_window.eX), easing_get_int(&mn_window.eY),
    easing_get_int(&mn_window.eW), easing_get_int(&mn_window.eH)
  );
  u8g2_SetDrawColor(mn_u8g2,1);
  u8g2_DrawFrame(mn_u8g2,
    easing_get_int(&mn_window.eX), easing_get_int(&mn_window.eY),
    easing_get_int(&mn_window.eW), easing_get_int(&mn_window.eH)
  );

  mn_item = &mn_picon->pxItems[mn_picon->uItemIndex];

  // #define infoPaddingX(i) ((MN_HOR_MID-u8g2_GetStrWidth(mn_u8g2,mn_item->xData.info[i]))/2)//居中
  #define infoPaddingX(i) (u8g2_GetMaxCharWidth(mn_u8g2))//左对齐
  #define infoPaddingY    ((MN_VER_RES-(u8g2_GetMaxCharHeight(mn_u8g2)*MN_ITEM_INFO_MAX))/2)//居中
  // #define infoPaddingY    (u8g2_GetMaxCharHeight(mn_u8g2))//上对齐
  
  #define infoMaxWidth    (MN_HOR_RES-u8g2_GetMaxCharWidth(mn_u8g2)*2)
  //限制u8g2绘制窗口
  int w = easing_get_int(&mn_window.eW)-1;
  w = w>3?w:3;
  u8g2_SetClipWindow(mn_u8g2,
    2,2,
    w,MN_VER_RES-1
  );

  switch( mn_get_itemType(mn_item) )
  {
    case MN_ITEM_TYPE_INFO:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("itemType:info");
      #endif
      //绘制info
      u8g2_SetFont(mn_u8g2,u8g2_font_5x7_tf);
      for(uint8_t i=0;i<MN_ITEM_INFO_MAX;i++)
      {
        u8g2_DrawStr(mn_u8g2,
          infoPaddingX(i),
          infoPaddingY+u8g2_GetMaxCharHeight(mn_u8g2)*(i+1),
          mn_item->xData.info[i]
        );
      }
      break;
    }
    case MN_ITEM_TYPE_VALUEINT:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("itemType:valueInt");
      #endif
      //绘制进度条-外侧框
      u8g2_SetDrawColor(mn_u8g2,1);
      u8g2_DrawFrame(mn_u8g2,
        piconProcAreaX+piconProcPaddingX,
        piconProcAreaY+piconProcPaddingY,
        piconProcW,
        piconProcH
      );
      //绘制进度条-内侧框
      u8g2_DrawFrame(mn_u8g2,
        piconProcAreaX+piconProcPaddingX-piconLineW/2,
        piconProcAreaY+piconProcPaddingY-piconLineW/2,
        piconProcW+piconLineW,
        piconProcH+piconLineW
      );
      //绘制进度条-进度
      u8g2_DrawBox(mn_u8g2,
        piconProcAreaX+piconProcPaddingX,
        piconProcAreaY+piconProcPaddingY+easing_get_int(&mn_window.eProcVal),
        piconProcW,
        easing_get_int(&mn_window.eProcVal)
      );
      break;
    }
    case MN_ITEM_TYPE_VALUEFLOAT:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("itemType:valueFloat");
      #endif


      break;
    }
    default:
    {
      break;
    }
  }// item switch end
  //恢复原字体
  u8g2_SetFont(mn_u8g2,mn_picon->pxParam->puFont);
  //恢复u8g2绘制窗口
  u8g2_SetMaxClipWindow(mn_u8g2);
}

void mn_window_painter(mn_handle_t handle, int arg_int)
{
  mn_page = (mn_page_t*)handle;

  #if MN_LOG_TRACE_WINDOW_RANDER
  MN_LOG_TRACE("start");
  MN_LOG_TRACE("page:%s",mn_page->pcTitle);
  #endif

  switch( mn_get_pageType(mn_page) )
  {
    case MN_PAGE_TYPE_LIST:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("pageType:list");
      #endif

      mn_window_painter_p_lsit(handle,arg_int);
      break;
    }
    case MN_PAGE_TYPE_ICON:
    {
      #if MN_LOG_TRACE_WINDOW_RANDER
      MN_LOG_TRACE("pageType:icon");
      #endif
      mn_window_painter_p_icon(handle,arg_int);
      break;
    }
    default:
    {
      break;
    }
  }
}