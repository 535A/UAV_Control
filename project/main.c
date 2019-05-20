/***************************** (C) COPYRIGHT NULL 小破孩 *************************

 * 文件名       ：
 * 描述         ：
 *
 * 实验平台     ：战舰V3开发板
 * 库版本       ：官方库   原子库  
 *
 * 作者         ：小破孩
 * 时间         ：2016.08.17
 * 说明         ：某些地方借鉴钱大神   系统时钟频率72Mhz

**********************************************************************************/

#include "com_def.h"


void run();//主执行函数
float abs(float i);
void update_oled();
void nrf_prepare();

u16 TimeCount=0,ad_count=0,nrf_count=0;
u8 tmp_buf[33];	 
u8 key,mode,oled_enter=0,oled_ex=0;
u16 t=0,U_f=0,U_speed=0;
u8 canshu=1,jiesuo=1,f_jz=0,f_cs=0;
int location=0;

u16 test_rrr=1;
u8 i=0;

u8 RX_buf[9]={0x11,0x00};
u8 TX_buf[9]={0};  //电压  欧拉角  油门  0.0 没了？
u8 Voltage=0;

u16 ad[4]={0};

void usart2_send_char(u8 c);

u16 Control_ROLL=50,Control_PITCH=50,Control_YAW=50,R_THROTTLE,start=0,correct=0,UAV_OK=0;   //遥控  偏航 俯仰 翻滚


void main()
{
  
  sys_config();
  //SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
  NRF24L01_TX_Mode();//主机调试 
  
  while(1)
  {
    ad[i]=(Get_Adc_Average(i+1,2))/40;
    i++;
    if(i==4)
      i=0;
  }
  
}
u8 adccl,hclk,pclk1,pclk2,sysclk;

void run()              //250hz  0.04s进一次
{
  
  TimeCount++;
 /* 
  ad_count++;
  nrf_count++;
  */
  //COUNT=~COUNT;
  nrf_prepare();
  
   delay_us(20);
 
    if(NRF24L01_TxPacket(TX_buf)==TX_OK)
    {
      if(canshu==1)
      OLED_ShowString(64,0,"OK");
      UAV_OK=1;
    }
    else
    {
      if(canshu==1)
      OLED_ShowString(64,0,"  ");
      UAV_OK=0;
      //start=0;
    }
  
  if(nrf_count==5)
  { 
     
    n2401_init();
    nrf_count=0;
    //NRF24L01_RX_Mode();
    delay_us(20);
       NRF24L01_TX_Mode();//主机调试 
    //NRF24L01_RxPacket(RX_buf);
     
  }
  
    update_oled();
 
  
 if(TimeCount==5)         //姿态解算
  {
    if(f_cs==0)
    {
      OLED_ShowChar(110,2,' '); 
      OLED_ShowChar(110,4,' '); 
      OLED_ShowChar(110,6,' '); 
      OLED_ShowChar(110,(location+1)*2,'*');
    }
    
    TimeCount=0;
   
  }
  
}

void scan()
{
  if(key_up==0)
    {
      //delay_ms(20);
      if(key_up==0)
      {
        if(location==0)
          location=0;
        else if(location==1)
          location=0;
        else if(location==2)
          location=1; 
        while(key_up==0);
      }
      if(location<=0) location=0;
      
    }
    
    if(key_down==0)
    {
      //delay_ms(20);
      if(key_down==0)
      {
        if(location==2)
          location=2;
        else if(location==1)
          location=2;
        else if(location==0)
          location=1;    
        while(key_down==0);
      }
      if(location>=2) location=2;
      
    }
  if(key_en==0)
  {
      if(key_en==0)
      {
        oled_enter=1;
        while(key_en==0);
      }
      if(location==0&&correct==0&&UAV_OK==1)
        correct=1;
      
      if(location==1&&canshu==1&&start==1&&UAV_OK==1)
        canshu=0;
          
      if(location==2&&start==0&&UAV_OK==1)
      {
        start=1;
        jiesuo=0;
      }
      else if(location==2&&start==1)
      {
        start=0;
        jiesuo=1;
        oled_offset();
      }
      
  }
  if(key_ex==0)
  {
      if(key_ex==0)
      {
        oled_ex=1;
        while(key_ex==0);
      }
      oled_offset();
     f_cs=0;
  }
  
  EXTI->PR=1<<11;  //清除中断标志位 
  EXTI->PR=1<<10;  //清除中断标志位 
  EXTI->PR=1<<8;  //清除中断标志位 
  EXTI->PR=1<<9;  //清除中断标志位 
}

void update_oled()
{
  if(start==1&&jiesuo==0)
  {
    OLED_ShowString(110,0,"  ");
    OLED_ShowCHinese(110,0,12);  //已  
    jiesuo=1;
  }
  
  if(correct==1&&canshu==1)
    OLED_ShowString(0,0,"DONE");
  
  if(canshu==0&&start==1)
  {
    if(f_cs==0)
    {
      OLED_Clear();
      OLED_ShowString(0,0,"POW");
      OLED_ShowString(0,2,"ROL");
      OLED_ShowString(0,4,"PIT");
      OLED_ShowString(0,6,"YAW");
    }
    
      OLED_ShowNum(60,0,ad[2],3,16);
      OLED_ShowNum(60,2,ad[1],3,16);
      OLED_ShowNum(60,4,ad[0],3,16);
      OLED_ShowNum(60,6,ad[3],3,16);
    
    f_cs=1;
  }

}

float abs_sz(float i)
{
  if(i>=0) return i;
  else return -i;
}

//串口1发送1个字符 
//c:要发送的字符
void usart2_send_char(u8 c)
{
	while((USART2->SR&0X40)==0);//等待上一次发送完毕   
        
	USART2->DR=c;   	
}

/*
TX_buf[1] 油门
TX_buf[2] ROLL
TX_buf[3] PITCH
TX_buf[4] YAW
*/
void nrf_prepare()
{
    u8 c=0;
    for(c=0;c<4;c++)
    {
      TX_buf[c+1]=ad[c];
    }
    
    TX_buf[5]=start;
    TX_buf[6]=correct;
}
