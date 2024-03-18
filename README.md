# MNUI

## 简介

这是一个基于U8g2的开源UI框架, 专用于单色显示屏。

目前处于初步开发阶段, 开发设备为STM32+SSD1306, 分辨率为128*64

绝大部分坐标都根据分辨率计算相对位置, 不同分辨率更改宏定义数值后可以自适应。

## 设计初衷

彩色LCD已经拥有的LVGL和TFT_eSPI等强大的中间件，但单色显示屏还没有一款可以快速移植部署，足以满足个人使用需求的UI框架，因此基于U8g2图库编写了此框架以满足我个人的日常使用需求。

## 开源协议

本软件使用GPLv3协议

- 你可以免费使用、复制、修改、分发受保护程序，但分发时一定要提供源码。

- 你可以收费。比如可以针对目标码、源码的分发收费，可以针对软件的维修保养收费；但不能收许可费、版税、专利许可费、授权费这类费用。

- GPL程序的所有贡献者，自动免费提供了专利许可，承诺不会告人侵犯专利权。

- GPL程序允许别人破解。如果是用在消费类硬件上，要允许人修改并安装程序。

## 代码或设计

本软件设计参考了

- 稚晖君

  UltraLink UI

  参考了部分界面的设计

- B站用户uYanki的项目

  https://github.com/uYanki/board-stm32f103rc-berial/

  参考了其UI部分的框架设计, 并做了大量修改。

- 缓动函数

  easing部分代码用于实现各种动画

  [缓动函数 Easing Functions-CSDN博客](https://blog.csdn.net/z2014z/article/details/120691794)

- LVGL

  使用了其日志系统部分的代码，LVGL代码基于MIT协议


本软件

以【页面page】和【页面选项item】为主要解析或操作的对象，

以【缓动变量easing】为实现动画特效的主要变量。

因为一般使用单色显示屏的微控制器RAM和Flash较为有限，屏幕分辨率有限，且easing变量占用内存较多，如果采用lvgl类似的obj结构，感觉内存和性能会不是很够，个人也没有思考出更好的动画实现或框架结构，最终根据以页面为操作对象的思想，做了大量重新设计后得到此框架。

目前只完成了少量功能

**页面类型**

- 列表菜单
- 图标菜单(128x64分辨率下 图标最大分辨率44x44)

**选项类型**

- 页面跳转
- 开关选项
- 整型数值选项
- 浮点数值选项
- 文本信息类型选项

**其他部件**

- 列表菜单(整型,浮点选项)弹出窗口
- 图标菜单(文本信息选项)弹出窗口

**后续还将添加**

- 列表菜单(文本信息选项弹出窗口
- 图标菜单(整型,浮点选项)弹出窗口

- 图表/波形页面
- 待机页面
- 更多的已有页面类型、窗口、指示器的样式
- 常见的光线传感器、温湿度传感器、ADC等数据绘制页面

## 可移植性

该框架有一定的可移植性，可以根据初始化选项列表的各参数直接生成对应风格的菜单，选项，窗口等。

- 初始化UI,绑定U8g2句柄

  ```c
  u8g2_init(&u8g2,&hi2c1,0x78);//u8g2初始化函数
  mn_init(&u8g2,0);//mnui初始化函数
  ```

- 设置【输入设备】的驱动类型和键值映射函数

  ```c
  /**
   * @brief 键值映射回调函数
   * @note  目前内部支持的键值
   *        MN_INDEV_KEY_ENTER 1
   *        MN_INDEV_KEY_UP    2
   *        MN_INDEV_KEY_DOWN  3
   *        MN_INDEV_KEY_BACK  4
  */
  uint8_t my_key_mapper(uint8_t val)
  {
    switch(val)
    {
      //case 键盘驱动输出键值:return mnui内部键值;
      case 'A':return MN_INDEV_KEY_ENTER;
      case 'B':return MN_INDEV_KEY_UP;
      case 'C':return MN_INDEV_KEY_DOWN;
      case 'D':return MN_INDEV_KEY_BACK;
    }
    return 0;
  }
  //输入设备初始化函数
  mn_indev_init(
    MN_INDEV_TYPE_IRQ,//中断驱动类型
    &key_val,//键值变量指针
    NULL,//轮询驱动类型需要的轮询回调函数
    my_key_mapper//键值映射函数
  );
  ```

- 以【页面】和【选项】为基础构想自己想要生成的菜单。

- 初始化完所有【页面】后在While(1)循环中调用mn_core_update()函数完成页面刷新



# 使用方法简介

## 图标菜单准备

```c
//页面句柄
mn_handle_t mnh_main;
//页面选项数组
mn_item_t mni_main[]={
  //选项1
  {
    .uType  = MN_ITEM_TYPE_PAGE,//选项类型:(跳转)另一个页面
    .pcText = "LED CTL",//选项标题
    
    //选项图标{位图数据指针,宽度,高度}
    .xIcon  = {											
      xbm_led,
      xbm_led_width,
      xbm_led_height
    },
  },
  {
    .uType  =  MN_ITEM_TYPE_INFO,//选项类型:纯文本信息
    .pcText = "ABOUT",//选项标题

    //选项图标{位图数据指针,宽度,高度}
    .xIcon  = 	{										
      xbm_question,
      xbm_question_width,
      xbm_question_height
    },
    .xData.info[0] = "STM32",//文本内容,最多显示6条
    .xData.info[1] = "F103RCT6",
    .xData.info[2] = "RAM:48KB",
    .xData.info[3] = "FLASH:256KB",
    .xData.info[4] = "U8G2",
    .xData.info[5] = "MNUI",
  }
};
```

### 列表菜单准备

```c
//选项1~3(switch)功能函数,开关对应颜色LED
void mnf_led_item1(mn_handle_t handle,int event)
{
  switch(event)
  {
    case 0:
      led_switch(LED_R,LED_OFF);
      break;
    case 1:
      led_switch(LED_R,LED_ON);
      break;
  }
}
void mnf_led_item2(mn_handle_t handle,int event);
void mnf_led_item3(mn_handle_t handle,int event);
//选项4(value int)功能函数,调节所有LED亮度
void mnf_led_item4(mn_handle_t handle,int event)
{
  mn_plist = (mn_p_list_t*)handle;//页面指针
  mn_item  = &mn_plist->pxItems[mn_plist->uItemIndex];//选项指针
  bsp_led_light(LED_R,mn_item->xData.sValue.val);//亮度设置函数,读取选项当前值
  bsp_led_light(LED_G,mn_item->xData.sValue.val);
  bsp_led_light(LED_B,mn_item->xData.sValue.val);
}
//页面句柄
mn_handle_t mnh_led;
//页面选项数组
mn_item_t mni_led[]={
  {
    .uType=MN_ITEM_TYPE_SWITCH,//选项类型:开关
    .pcText = "RED",//选项文本
    .lpfnFunction = mnf_led_item1//选项回调函数
  },
  {
    .uType=MN_ITEM_TYPE_SWITCH,
    .pcText = "GREEN",
    .lpfnFunction = mnf_led_item2
  },
  {
    .uType=MN_ITEM_TYPE_SWITCH,
    .pcText = "BLUE",
    .lpfnFunction = mnf_led_item3
  },
  {
    .uType=MN_ITEM_TYPE_VALUEINT,//选项类型:整数值
    .pcText = "LIGHT",             
    .xData.sValue={50,0,100,10},//选项数据:初始值,最小值,最大值,分割数
    .lpfnFunction = mnf_led_item4//选项回调函数
  }
};
```

## 页面初始化

```c
void main()
{
  //0.初始化硬件,u8g2,以及mnui核心
  u8g2_init(&u8g2,&hi2c1,0x78);//u8g2初始化函数
  mn_init(&u8g2,0);//mnui初始化函数
  
  //1.初始化各个页面,申请内存,初始化坐标参数,传入选项数组等
	mnh_main = mn_p_icon_create(
    "ICON",//菜单标题
    mni_main,//选项数组
    ARRAY_COUNT(mni_main),//选项数
    NULL//样式选择参数
  );
  mnh_led = mn_p_list_create(
    "MAIN",
    mni_led,
    ARRAY_COUNT(mni_led),
    NULL
  );
  
  //2.设置页面之间的关系
  mn_set_pageRelationship(
    mnh_main,//父页面句柄
    0,//父页面的选项索引
    mnh_led//子页面句柄
  );
  
  //3.切换至某个页面,作为初试页面
  mn_switch_page(mnh_main);
  while(1)
  {
    mn_update();//4.刷新页面
	}
}
```

