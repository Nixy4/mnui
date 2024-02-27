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

#ifndef __MNUI_H__
#define __MNUI_H__

#include "./mn_conf.h"

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

#include "./easing.h"

//! 日志
#define MN_LOG_LEVEL_TRACE 0
#define MN_LOG_LEVEL_INFO  1
#define MN_LOG_LEVEL_WARN  2
#define MN_LOG_LEVEL_ERROR 3
#define MN_LOG_LEVEL_USER  4
#define MN_LOG_LEVEL_NONE  5
#define _MN_LOG_LEVEL_NUM  6

#if MN_USE_LOG

  void _mn_log_add(int level, const char* file, uint32_t line, const char* func, const char* fmt, ...);

  #if MN_LOG_LEVEL <= MN_LOG_LEVEL_TRACE
    #define MN_LOG_TRACE(...) _mn_log_add(MN_LOG_LEVEL_TRACE,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
  #else
    #define MN_LOG_TRACE(...)
  #endif

  #if MN_LOG_LEVEL <= MN_LOG_LEVEL_INFO
    #define MN_LOG_INFO(...) _mn_log_add(MN_LOG_LEVEL_INFO,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
  #else
    #define MN_LOG_INFO(...)
  #endif

  #if MN_LOG_LEVEL <= MN_LOG_LEVEL_WARN
    #define MN_LOG_WARN(...) _mn_log_add(MN_LOG_LEVEL_WARN,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
  #else
    #define MN_LOG_WARN(...)
  #endif

  #if MN_LOG_LEVEL <= MN_LOG_LEVEL_ERROR
    #define MN_LOG_ERROR(...) _mn_log_add(MN_LOG_LEVEL_ERROR,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
  #else
    #define MN_LOG_ERROR(...)
  #endif

  #if MN_LOG_LEVEL <= MN_LOG_LEVEL_USER
    #define MN_LOG_USER(...) _mn_log_add(MN_LOG_LEVEL_USER,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
  #else
    #define MN_LOG_USER(...)
  #endif

#else
  #define _mn_log_add(level, file, line, function, format, ...) 
  #define MN_LOG_TRACE(...)
  #define MN_LOG_INFO(...)
  #define MN_LOG_WARN(...)
  #define MN_LOG_ERROR(...)
  #define MN_LOG_USER(...)
#endif

//! 工具宏
#define MN_HOR_MID      (((float)MN_HOR_RES)/2.f) 
#define MN_VER_MID      (((float)MN_VER_RES)/2.f)
#define ARRAY_COUNT(x)  (sizeof(x)/sizeof(x[0]))

//! 工作状态
#define MN_STATE_PAGE_SWITCHING    0U
#define MN_STATE_PAGE_NORMAL       1U
#define MN_STATE_WINDOW_CLOSED     1U
#define MN_STATE_WINDOW_CLOSING    2U
#define MN_STATE_WINDOW_OPENED     3U
#define MN_STATE_WINDOW_OPENING    4U

//! 页面句柄
typedef void* mn_handle_t;
//! 函数类型
typedef void (*mn_function_t)(void* arg_ptr,int style);

//! 输入设备
//输入设备工作模式
#define MN_INDEV_TYPE_POLL 0x01 //轮询模式
#define MN_INDEV_TYPE_IRQ  0x02 //中断模式
//内部支持的键值
#define MN_INDEV_KEY_ENTER 1
#define MN_INDEV_KEY_BACK  2
#define MN_INDEV_KEY_UP    3
#define MN_INDEV_KEY_DOWN  4
// #define MN_INDEV_KEY_LEFT  5
// #define MN_INDEV_KEY_RIGHT 6
//输入设备函回调函数类型
typedef uint8_t (*mn_indev_callback)(uint8_t);
//输入设备结构
typedef struct _mn_indev_struct
{
  uint8_t           uType;       //输入设备驱动类型
  mn_indev_callback lpfnMapper;  //键值映射函数
  mn_indev_callback lpfnGetVal;  //键值获取函数
  volatile uint8_t* pvuVal;      //硬件驱动键值指针
  uint32_t          bEnable;     //输入设备使能
  uint8_t           puValBuf;    //键值缓存
}mn_indev_t;

//! 参数
typedef struct _mn_param_struct
{
  uint8_t           uFrame;         //帧数
  easing_calc_fn    lpfnEasingCalc; //缓动计算函数
  const uint8_t*    puFont;         //字体
}mn_param_t;

//! 图标
typedef struct _mn_icon_struct
{
  const uint8_t* pcuData;
  uint16_t       uWidth;
  uint16_t       uHeight;
}mn_icon_t;

//! 指示器
typedef struct _mn_pointer_struct
{
  uint32_t    bEnable;    //指针使能
  mn_param_t* pxParam;    //参数
  easing      eX;         //x轴缓动值
  easing      eY;         //y轴缓动值
  easing      eW;         //宽度缓动值
  easing      eH;         //高度缓动值
  uint8_t     uR;         //半径
}mn_pointer_t;

//! 选项
//页面项类型
#define MN_ITEM_TYPE_NONE       0U
#define MN_ITEM_TYPE_INFO       1U
#define MN_ITEM_TYPE_PAGE       2U
#define MN_ITEM_TYPE_SWITCH     3U
#define MN_ITEM_TYPE_VALUEINT   4U
#define MN_ITEM_TYPE_VALUEFLOAT 5U

#define MN_ITEM_INFO_MAX 6

//页面项数据结构
typedef struct _mn_item_struct
{

  uint8_t       uType;           //项类型
  const char*   pcText;          //文本
  mn_icon_t     xIcon;           //图标
  mn_function_t lpfnFunction;    //功能函数

  union
  {
    const char*                                               info[MN_ITEM_INFO_MAX];  //文本信息
    mn_handle_t                                               xChild;   //子页面
    bool                                                      bValue;   //switch值
    struct{int val;int min;int max;uint8_t blockCount;}       sValue;   //int值
    struct{float val;float min;float max;uint8_t blockCount;} fValue;   //float值
  }xData;

}mn_item_t;

//! 窗口
//窗口数据结构
typedef struct _mn_window_struct
{
  const char*       pcText;        //标题
  easing            eX;            //x轴缓动值
  easing            eY;            //y轴缓动值
  easing            eW;            //宽度缓动值
  easing            eH;            //高度缓动值
  easing            eProcW;        //进度条宽度缓动值
  mn_param_t*       pxParam;       //参数
}mn_window_t;

//! 页面基本数据
#define MN_MEM_TYPE_MASK     0xF0U //页面内存掩码
#define MN_MEM_TYPE_STATIC   0x10U //静态内存
#define MN_MEM_TYPE_DYNAMI   0x20U //动态内存

#define MN_PAGE_TYPE_MASK    0x0FU //页面类型掩码
#define MN_PAGE_TYPE_CUSTOM  0x01U //自定义页面
#define MN_PAGE_TYPE_LIST    0x02U //列表页面
#define MN_PAGE_TYPE_ICON    0x03U //图标页面

typedef struct _mn_page_struct
{

  uint8_t     uType;            //页面类型
  uint16_t    uTypeSize;        //页面类型大小
  const char* pcTitle;          //页面标题
  mn_param_t* pxParam;          //页面参数
  mn_handle_t pxParent;         //父页面
  easing      eX;               //x轴缓动值
  easing      eY;               //y轴缓动值
  mn_item_t*  pxItems;          //列表项
  uint32_t    uItemCount;       //列表项数量
  uint8_t     uItemIndex;       //列表项索引
}mn_page_t;

//! 自定义页面
typedef struct _mn_p_custom_struct
{
  //mn_page_t成员
  uint8_t       uType;            //页面类型
  uint16_t      uTypeSize;        //页面类型大小
  const char*   pcTitle;          //页面标题
  mn_param_t*   pxParam;          //页面参数
  mn_handle_t   pxParent;         //父页面
  easing        eX;               //x轴缓动值
  easing        eY;               //y轴缓动值
  void*         pvArgPtr;         //指针参数
  int           sArgInt;          //数值参数
  //
  mn_function_t lpfnAppear;       //页面出现函数
  mn_function_t lpfnDisappear;    //页面消失函数
  mn_function_t lpfnHandler;      //页面处理函数
  mn_function_t lpfnPainter;      //页面绘制函数

}mn_p_custom_t;

//! 列表页面
#define MN_PAGE_LIST_POINTER2_WIDTH  5U
//页面样式
#define MN_PAGE_LIST_STYLE_0         0U
#define MN_PAGE_LIST_STYLE_1         1U
#define MN_PAGE_LIST_STYLE_2         2U


typedef struct _mn_p_list_struct
{
  //mn_page_t成员
  uint8_t     uType;            //页面类型
  uint16_t    uTypeSize;        //页面类型大小
  const char* pcTitle;          //页面标题
  mn_param_t* pxParam;          //页面参数
  mn_handle_t pxParent;         //父页面
  easing      eX;               //x轴缓动值
  easing      eY;               //y轴缓动值
  mn_item_t*  pxItems;          //列表项
  uint32_t    uItemCount;       //列表项数量
  uint8_t     uItemIndex;       //列表项索引
  //
  uint8_t     uPointerIndex;    //指示器索引
  uint8_t     uItemDispCount;   //列表项显示数量
  float       fItemHeight;      //列表项高度
  easing      eItemTop;         //列表项顶部缓动值
  easing      eItemOffset;      //列表项偏移缓动值
  float       fFontHeight;      //字体高度
  float       fPaddingX;        //x轴内边距
  float       fPaddingY;        //y轴内边距
  float       fPointer2Height;  //指示器高度
}mn_p_list_t;

//! 图标页面
typedef struct _mn_p_icon_struct
{
  //mn_page_t成员
  uint8_t     uType;            //页面类型
  uint16_t    uTypeSize;        //页面类型大小
  const char* pcTitle;          //页面标题
  mn_param_t* pxParam;          //页面参数
  mn_handle_t pxParent;         //父页面
  easing      eX;               //x轴缓动值
  easing      eY;               //y轴缓动值
  mn_item_t*  pxItems;          //图标项
  uint32_t    uItemCount;       //图标项数量
  uint8_t     uItemIndex;       //图标项索引
  //
  float       fItemSide;        //图标项边长
  float       fFontHeight;      //字体高度
  float       fItemInterval;    //图标项间隔
  float       fIconRiseMax;     //图标上升最大值
  easing      eItemX;           //图标项x轴缓动值
  easing      eItemY;           //图标项y轴缓动值
  easing      eItemTop;         //图标项顶部缓动值
  easing      eItemOffset;      //图标项偏移缓动值
  easing      eTextRise;        //文本上升缓动
  easing      eIconRise;        //图标上升缓动
}mn_p_icon_t;

//! indev.c
//输入设备初始化函数
void mn_indev_init(
  uint8_t           uType,      //输入设备驱动类型
  volatile uint8_t* pvuVal,     //硬件驱动键值指针
  mn_indev_callback       lpfnGetVal, //键值获取函数
  mn_indev_callback       lpfnMapper  //键值映射函数
  );
void    mn_indev_update(void);//键值更新函数
uint8_t mn_indev_getBuff(void);//获取键值缓存
void    mn_indev_clearBuff(void);//清空键值缓存
#define mn_indev_enable()  mn_indev.bEnable = 1
#define mn_indev_disable() mn_indev.bEnable = 0

//! pointer.c
void mn_pointer_init(mn_pointer_t* p,mn_param_t* pa,uint8_t r);
void mn_pointer_set_param(mn_pointer_t* p,mn_param_t* pa,uint8_t r);

void mn_pointer_enable(mn_pointer_t* p);
void mn_pointer_disable(mn_pointer_t* p);

void mn_pointer_moveAbsolut(mn_pointer_t* p,float x1,float y1,float x2,float y2);
void mn_pointer_moveTarget(mn_pointer_t* p,float x,float y);
void mn_pointer_moveRelative(mn_pointer_t* p,float x,float y);

void mn_pointer_resizeAbsolut(mn_pointer_t* p,float w1,float h1,float w2,float h2);
void mn_pointer_resizeTarget(mn_pointer_t* p,float w,float h);
void mn_pointer_resizeRelative(mn_pointer_t* p,float w,float h);

void mn_pointer_handler(mn_pointer_t* p,mn_page_t* page);
void mn_pointer_painter(mn_pointer_t* p,mn_page_t* page);

//! window.c
void mn_window_init();
void mn_window_appear(mn_handle_t handle, int style);
void mn_window_disappear(mn_handle_t handle, int style);
void mn_window_handler(mn_handle_t handle, int style);
void mn_window_painter(mn_handle_t handle, int style);

//! p_custom.c
mn_handle_t mn_p_list_creatGeneral(
  const char*       title,
  mn_p_list_t*      page,
  uint32_t          memType,
  mn_item_t*        items,
  uint32_t          itemCount,
  mn_param_t*       param
  );
#define mn_p_list_create(title,items,itemCount,param) \
  mn_p_list_creatGeneral(title,NULL,MN_MEM_TYPE_DYNAMI,items,itemCount,param)
#define mn_p_list_createStatic(title,page,items,itemCount,param) \
  mn_p_list_creatGeneral(title,page,MN_MEM_TYPE_STATIC,items,itemCount,param)

void mn_p_list_appear(mn_p_list_t* page,int style);
void mn_p_list_disappear(mn_p_list_t* page,int style);
void mn_p_list_handler(mn_p_list_t* page,int style);
void mn_p_list_painter(mn_p_list_t* page,int style);

//! p_icon.c
mn_handle_t mn_p_icon_createGeneral(
  const char*       title,
  mn_p_icon_t*      page,
  uint32_t          memType,
  mn_item_t*        items,
  uint16_t          itemCount,
  mn_param_t*       param
  );
#define mn_p_icon_create(title,items,itemCount,param) \
  mn_p_icon_createGeneral(title,NULL,MN_MEM_TYPE_DYNAMI,items,itemCount,param)
#define mn_p_icon_createStatic(title,page,items,itemCount,param) \
  mn_p_icon_createGeneral(title,page,MN_MEM_TYPE_STATIC,items,itemCount,param)  

void mn_p_icon_appear(mn_p_icon_t* page,int style);
void mn_p_icon_disappear(mn_p_icon_t* page,int style);
void mn_p_icon_handler(mn_p_icon_t* page,int style);
void mn_p_icon_painter(mn_p_icon_t* page,int style);

//! core.c
void mn_init(u8g2_t* u8g2, int arg_int);
void mn_update(void);

void mn_repaint_fromEasing(easing* e);
void mn_repaint_fromValue(void);
void mn_repaint_fromWindow(void);

void mn_switch_page(mn_handle_t handle);
void mn_set_pageRelationship(mn_handle_t parent,uint8_t itemIndex,mn_handle_t child);

void mn_window_open(mn_handle_t handle);
void mn_window_close(mn_handle_t handle);

uint8_t mn_get_itemType(mn_item_t* item);
uint8_t mn_get_pageType(mn_page_t* page);

//! misc.c
void mn_disappear_dissolve(void);
void mn_disappear_dissolve_local(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void mn_disappear_crtClose(void);

int  mn_ftoi(float val);
void mn_play_animation(const uint8_t* gif[],uint16_t cnt,uint16_t delay,uint16_t x,uint16_t y,uint16_t w, uint16_t h);

//! core.c
extern u8g2_t*        mn_u8g2;
extern uint32_t       mn_state;

extern mn_page_t*     mn_pageCurr;
extern mn_page_t*     mn_pagePrev;

extern mn_page_t*     mn_page;
extern mn_item_t*     mn_item;
extern mn_p_list_t*   mn_plist;
extern mn_p_icon_t*   mn_picon;
extern mn_p_custom_t* mn_pcustom;

extern mn_param_t     mn_defaultParam;

//! indev.c
extern mn_indev_t     mn_indev;

//! pointer.c
extern mn_pointer_t   mn_pointer1;
extern mn_pointer_t   mn_pointer2;

//! window.c
extern mn_window_t    mn_window;

#endif