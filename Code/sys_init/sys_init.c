/***************************** (C) COPYRIGHT NULL 小破孩 *************************

 * 文件名       ：
 * 描述         ：初始化
 *
 * 作者         ：小破孩
 * 说明         ：借鉴钱大神 官方库 原子库

**********************************************************************************/
#include "com_def.h"


//0.04ms控制一次 周期

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
            run();			    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}


void EXTI9_5_IRQHandler(void)
{
  /*
    NRF24L01_RxPacket(RX_buf);
    OLED_ShowNum(70,0,RX_buf[2],3,16);   
    OLED_ShowNum(70,2,RX_buf[1],3,16);   
    OLED_ShowNum(70,4,RX_buf[0],3,16);  
    OLED_ShowNum(70,6,RX_buf[3],3,16);
    
    NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除TX FIFO寄存器 
  */
    
    scan();

}

void EXTI15_10_IRQHandler(void)
{
  
      scan();
      //EXTI->PR=1<<7;  //清除中断标志位 
}

void TIM3_init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
    TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
    TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
    TIM3->DIER|=1<<0;   //允许更新中断	  
    TIM3->CR1|=0x01;    //使能定时器3
    MY_NVIC_Init(1,1,TIM3_IRQn,2);//抢占1，子优先级3，组2	
}


void SPI2_init()
{
  
  RCC->APB2ENR|=1<<3;  	//PORTB时钟使能 	 
  RCC->APB1ENR|=1<<14;   	//SPI2时钟使能 
	//这里只针对SPI口初始化
  GPIOB->CRH&=0X000FFFFF; 
  GPIOB->CRH|=0XBBB00000;	//PB13/14/15复用 	    
  GPIOB->ODR|=0X7<<13;   	//PB13/14/15上拉
  SPI2->CR1|=0<<10;		//全双工模式	
  SPI2->CR1|=1<<9; 		//软件nss管理
  SPI2->CR1|=1<<8;  

  SPI2->CR1|=1<<2; 		//SPI主机
  SPI2->CR1|=0<<11;		//8bit数据格式	
  SPI2->CR1|=1<<1; 		//空闲模式下SCK为1 CPOL=1
  SPI2->CR1|=1<<0; 		//数据采样从第二个时间边沿开始,CPHA=1  
	//对SPI2属于APB1的外设.时钟频率最大为36M.
  SPI2->CR1|=3<<3; 		//Fsck=Fpclk1/256   ？  011是Fpclk1/16
  SPI2->CR1|=0<<7; 		//MSBfirst   
  SPI2->CR1|=1<<6; 		//SPI设备使能
  SPI2_ReadWriteByte(0xff);//启动传输	
  
}

/*  
void SPI1_init()  //从机  外接
{
  RCC->APB2ENR|=1<<2;  	//PORTA时钟使能 	 
  RCC->APB2ENR|=1<<12;   	//SPI1时钟使能 
          //这里只针对SPI口初始化
  GPIOA->CRL&=0X000FFFFF; 
  GPIOA->CRL|=0XBBB00000;	//PA5/6/7复用 	    
  GPIOA->ODR|=0X7<<5;   	//PA5/6/7上拉
  SPI1->CR1|=0<<10;		//全双工模式	
  SPI1->CR1|=1<<9; 		//软件nss管理
  SPI1->CR1|=1<<8;  

  SPI1->CR1|=1<<2; 		//SPI主机
  SPI1->CR1|=0<<11;		//8bit数据格式	
  SPI1->CR1|=1<<1; 		//空闲模式下SCK为1 CPOL=1
  SPI1->CR1|=1<<0; 		//数据采样从第二个时间边沿开始,CPHA=1  
	//对SPI2属于APB1的外设.时钟频率最大为36M.
  SPI1->CR1|=3<<3; 		//Fsck=Fpclk1/256   ？  011是Fpclk1/16
  SPI1->CR1|=0<<7; 		//MSBfirst   
  SPI1->CR1|=1<<6; 		//SPI设备使能
  SPI1_ReadWriteByte(0xff);//启动传输	
  
}
*/

void n2401_init()
{
  
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,G端口时钟
   
//接收的2401          UAV  B 5 6 7
 RCC->APB2ENR|=1<<3;  	//PORTB时钟使能 
  
  GPIOB->CRL&=0X000FFFFF;       //PB7上/下拉输入,PB6推挽输出 PB5推挽输出
  GPIOB->CRL|=0X83300000;		
  GPIOB->ODR|=7<<5;     	//PB5 6 7 上拉
/*   
  GPIOB->CRH&=0XFFF0FFFF; 
  GPIOB->CRH|=0X00030000;	//PB12 推挽 	    
  GPIOB->ODR|=1<<12;    	//PB12上拉   摄像头会用么？
 
  
  RCC->APB2ENR|=1<<3;  	//PORTB时钟使能 
  GPIOB->CRH&=0XFFF0FFFF;         //就是GPIOB_CRH  将PB12配置为推挽输出模式 最大速度2Mhz   
  GPIOB->CRH|=0X00030000;
  GPIOB->ODR|=1<<12;    	//PB12上拉 防止W25Qxx的干扰 
  
  RCC->APB2ENR|=1<<8;		//使能PORTG时钟 
  GPIOG->CRL&=0X00FFFFFF;       //PG6上/下拉输入,PG7推挽输出
  GPIOG->CRL|=0X38000000;	
  GPIOG->CRH&=0XFFFFFFF0;
  GPIOG->CRH|=0X00000003;	//PG8 推挽输出 	    
  GPIOG->ODR|=7<<6;     	//PG6 7 8 上拉	
 */  
  
   SPI2_init();          //配置SPI2
   
  SPI2->CR1&=~(1<<6); 	//SPI设备失能
  SPI2->CR1&=~(1<<1); 	//空闲模式下SCK为0 CPOL=0
  SPI2->CR1&=~(1<<0); 	//数据采样从第1个时间边沿开始,CPHA=0  
  SPI2->CR1|=1<<6; 		//SPI设备使能
  
  NRF24L01_CE=0; 			//使能24L01
  NRF24L01_CSN=1;			//SPI片选取消
  //delay_ms(200);
  
  /*
     //发射的2401
  
  
  GPIOG->CRL&=0X00FFFFFF;       //PG6上/下拉输入,PG7推挽输出
  GPIOG->CRL|=0X38000000;	
  GPIOG->CRH&=0XFFFFFFF0;
  GPIOG->CRH|=0X00000003;	//PG8 推挽输出 	    
  GPIOG->ODR|=7<<6;     	//PG6 7 8 上拉	 
  
  
  SPI2_init();                   //配置SPI2
  
  //针对NRF的特点修改SPI的设置
  SPI2->CR1&=~(1<<6); 	//SPI设备失能
  SPI2->CR1&=~(1<<1); 	//空闲模式下SCK为0 CPOL=0
  SPI2->CR1&=~(1<<0); 	//数据采样从第1个时间边沿开始,CPHA=0  
  SPI2->CR1|=1<<6; 		//SPI设备使能
 
  NRF24L01_M_CE=0; 			//使能24L01
  NRF24L01_M_CSN=1;			//SPI片选取消	
  */
  
}

void LED_init()    
{      
  RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
  RCC->APB2ENR|=1<<6;    //使能PORTE时钟	
                   
  GPIOB->CRL&=0XFF0FFFFF; 
  GPIOB->CRL|=0X00300000;//PB.5 推挽输出   	 
  GPIOB->ODR|=1<<5;      //PB.5 输出高
                                                                                            
  GPIOE->CRL&=0XFF0FFFFF;
  GPIOE->CRL|=0X00300000;//PE.5推挽输出
  GPIOE->ODR|=1<<5;      //PE.5输出高 
  
}



void EXIT_init()
{
  
   Ex_NVIC_Config(GPIO_B,10,FTIR); 	//B.10  外部中断 下降沿触发
   Ex_NVIC_Config(GPIO_B,11,FTIR); 	//B.10  外部中断 下降沿触发
   Ex_NVIC_Config(GPIO_B,8,FTIR); 	//B.10  外部中断 下降沿触发
   Ex_NVIC_Config(GPIO_B,9,FTIR); 	//B.10  外部中断 下降沿触发
   
   MY_NVIC_Init(1,3,EXTI15_10_IRQn,2);//抢占1，子优先级3，组2	
  MY_NVIC_Init(1,3,EXTI9_5_IRQn,2);//抢占1，子优先级3，组2
   
}


void key_init()
{
  RCC->APB2ENR|=1<<3;    //使能PORTB时钟
  GPIOB->CRH&=0XFFFF00FF; 
  GPIOB->CRH|=0X00008800;//PB.10 11 输入   	 
  GPIOB->ODR|=1<<10;    	//PB10 上拉
  GPIOB->ODR|=1<<11;    	//PB11
  
  
  GPIOB->CRH&=0XFFFFFF00; 
  GPIOB->CRH|=0X00000088;//PB.8 9 推挽输出   	 
  GPIOB->ODR|=1<<9;      //PB.9 输出高
  GPIOB->ODR|=1<<8;      //PB.9 输出高
  
  
}

void sys_config()
{
  //Stm32_Clock_Init(9);
  //SystemInit();
  delay_init();
  
  //delay_ms(4000);  //delay 4s
  
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级    
  
  //uart_init(230400);   //115200
  //RCC->APB2ENR&=0xFFFFFBFF;    //关掉uart1时钟
  
  key_init();
  
  n2401_init();
  NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
  NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
  while(NRF24L01_Check());
  
  //NRF24L01_RX_Mode();//主机调试
  
  //MPU_Init();
 // HMC5883_init();
  //LED_init();
  
  SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
 
  
  Adc_Init();
  OLED_Init();
  
  OLED_ShowString(0,0,"XPH_UAV");  //NAME
  delay_ms(500);

  oled_offset();
   
  EXIT_init();
  
  
  TIM3_init(19,7199);  //6ms
  
}











