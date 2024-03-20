#include "stm32_hal_hw_i2c_u8g2.h"

static I2C_HandleTypeDef* hi2cx;
static uint8_t addrx;

uint8_t u8x8_byte_hw_i2c(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
  static uint8_t buffer[32];
  static uint8_t buf_idx;
  uint8_t *data;
  switch (msg) 
  {
    //2.准备要发送的数据,复制到缓冲中
    case U8X8_MSG_BYTE_SEND:
    {
      data=(uint8_t *)arg_ptr;      
      while(arg_int>0)
      {
        buffer[buf_idx++]=*data;
        data++;
        arg_int--;
      } 
      break;
    }
    //1.传输开始执行的任务,发送前清空索引
    case U8X8_MSG_BYTE_START_TRANSFER:
    {
      buf_idx=0;
      break;
    }
    //3.传输结尾执行的任务,把处理好的数据通过buffer传送给i2c接口
    case U8X8_MSG_BYTE_END_TRANSFER:
    {
      // M3的I2C接口速度太慢,使用DMA会导致第二次发送数据时,第一次的数据还没发送完毕,导致第二次发送失败
      if (HAL_I2C_Master_Transmit(hi2cx,addrx,buffer,buf_idx,0xFF)!=HAL_OK) 
        return 0;
      break;
    }
    default:
    {
      return 0;
    }
  }
  return 1;
}

uint8_t u8x8_gpio_and_delay(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
  #if 0
  switch (msg) 
  {
      //使用硬件I2C接口不需要延时函数
      case U8X8_MSG_DELAY_100NANO:      __nop();break;
      case U8X8_MSG_DELAY_10MICRO:      delay_us(10);break;
      case U8X8_MSG_DELAY_MILLI:        delay_ms(1);break;
      case U8X8_MSG_DELAY_I2C:          for(int i=0;i<13;i++){__nop();};break;

      //没有使用U8G2自带的按键功能,所以不需要处理
      case U8X8_MSG_GPIO_MENU_SELECT:   u8x8_SetGPIOResult(u8x8,0);break;
      case U8X8_MSG_GPIO_MENU_NEXT:     u8x8_SetGPIOResult(u8x8,0);break;
      case U8X8_MSG_GPIO_MENU_PREV:     u8x8_SetGPIOResult(u8x8,0);break;
      case U8X8_MSG_GPIO_MENU_HOME:     u8x8_SetGPIOResult(u8x8,0);break;
      default:                          u8x8_SetGPIOResult(u8x8,1);break;
  }
  #endif
  return 1;
}

void u8g2_init(u8g2_t* u8g2,I2C_HandleTypeDef* hi2c,uint8_t addr)
{
  hi2cx=hi2c;
  addrx=addr;
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_and_delay);
  u8g2_InitDisplay(u8g2);
  u8g2_SetPowerSave(u8g2,0);
  u8g2_ClearBuffer(u8g2);
  u8g2_SetFont(u8g2,u8g2_font_6x10_tf);
  char buf[] = "u8g2 Init End!";
  // u8g2_DrawStr(u8g2,63-u8g2_GetStrWidth(u8g2,buf)/2,31-u8g2_GetStrWidth(u8g2,buf)/2,buf);
  u8g2_DrawStr(u8g2,0,10,buf);
  u8g2_SendBuffer(u8g2);
}
