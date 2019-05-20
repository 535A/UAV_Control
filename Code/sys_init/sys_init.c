/***************************** (C) COPYRIGHT NULL С�ƺ� *************************

 * �ļ���       ��
 * ����         ����ʼ��
 *
 * ����         ��С�ƺ�
 * ˵��         �����Ǯ���� �ٷ��� ԭ�ӿ�

**********************************************************************************/
#include "com_def.h"


//0.04ms����һ�� ����

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
            run();			    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}


void EXTI9_5_IRQHandler(void)
{
  /*
    NRF24L01_RxPacket(RX_buf);
    OLED_ShowNum(70,0,RX_buf[2],3,16);   
    OLED_ShowNum(70,2,RX_buf[1],3,16);   
    OLED_ShowNum(70,4,RX_buf[0],3,16);  
    OLED_ShowNum(70,6,RX_buf[3],3,16);
    
    NRF24L01_Write_Reg(FLUSH_RX,0xff);//���TX FIFO�Ĵ��� 
  */
    
    scan();

}

void EXTI15_10_IRQHandler(void)
{
  
      scan();
      //EXTI->PR=1<<7;  //����жϱ�־λ 
}

void TIM3_init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
    TIM3->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
    TIM3->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
    TIM3->DIER|=1<<0;   //��������ж�	  
    TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
    MY_NVIC_Init(1,1,TIM3_IRQn,2);//��ռ1�������ȼ�3����2	
}


void SPI2_init()
{
  
  RCC->APB2ENR|=1<<3;  	//PORTBʱ��ʹ�� 	 
  RCC->APB1ENR|=1<<14;   	//SPI2ʱ��ʹ�� 
	//����ֻ���SPI�ڳ�ʼ��
  GPIOB->CRH&=0X000FFFFF; 
  GPIOB->CRH|=0XBBB00000;	//PB13/14/15���� 	    
  GPIOB->ODR|=0X7<<13;   	//PB13/14/15����
  SPI2->CR1|=0<<10;		//ȫ˫��ģʽ	
  SPI2->CR1|=1<<9; 		//���nss����
  SPI2->CR1|=1<<8;  

  SPI2->CR1|=1<<2; 		//SPI����
  SPI2->CR1|=0<<11;		//8bit���ݸ�ʽ	
  SPI2->CR1|=1<<1; 		//����ģʽ��SCKΪ1 CPOL=1
  SPI2->CR1|=1<<0; 		//���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	//��SPI2����APB1������.ʱ��Ƶ�����Ϊ36M.
  SPI2->CR1|=3<<3; 		//Fsck=Fpclk1/256   ��  011��Fpclk1/16
  SPI2->CR1|=0<<7; 		//MSBfirst   
  SPI2->CR1|=1<<6; 		//SPI�豸ʹ��
  SPI2_ReadWriteByte(0xff);//��������	
  
}

/*  
void SPI1_init()  //�ӻ�  ���
{
  RCC->APB2ENR|=1<<2;  	//PORTAʱ��ʹ�� 	 
  RCC->APB2ENR|=1<<12;   	//SPI1ʱ��ʹ�� 
          //����ֻ���SPI�ڳ�ʼ��
  GPIOA->CRL&=0X000FFFFF; 
  GPIOA->CRL|=0XBBB00000;	//PA5/6/7���� 	    
  GPIOA->ODR|=0X7<<5;   	//PA5/6/7����
  SPI1->CR1|=0<<10;		//ȫ˫��ģʽ	
  SPI1->CR1|=1<<9; 		//���nss����
  SPI1->CR1|=1<<8;  

  SPI1->CR1|=1<<2; 		//SPI����
  SPI1->CR1|=0<<11;		//8bit���ݸ�ʽ	
  SPI1->CR1|=1<<1; 		//����ģʽ��SCKΪ1 CPOL=1
  SPI1->CR1|=1<<0; 		//���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	//��SPI2����APB1������.ʱ��Ƶ�����Ϊ36M.
  SPI1->CR1|=3<<3; 		//Fsck=Fpclk1/256   ��  011��Fpclk1/16
  SPI1->CR1|=0<<7; 		//MSBfirst   
  SPI1->CR1|=1<<6; 		//SPI�豸ʹ��
  SPI1_ReadWriteByte(0xff);//��������	
  
}
*/

void n2401_init()
{
  
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,G�˿�ʱ��
   
//���յ�2401          UAV  B 5 6 7
 RCC->APB2ENR|=1<<3;  	//PORTBʱ��ʹ�� 
  
  GPIOB->CRL&=0X000FFFFF;       //PB7��/��������,PB6������� PB5�������
  GPIOB->CRL|=0X83300000;		
  GPIOB->ODR|=7<<5;     	//PB5 6 7 ����
/*   
  GPIOB->CRH&=0XFFF0FFFF; 
  GPIOB->CRH|=0X00030000;	//PB12 ���� 	    
  GPIOB->ODR|=1<<12;    	//PB12����   ����ͷ����ô��
 
  
  RCC->APB2ENR|=1<<3;  	//PORTBʱ��ʹ�� 
  GPIOB->CRH&=0XFFF0FFFF;         //����GPIOB_CRH  ��PB12����Ϊ�������ģʽ ����ٶ�2Mhz   
  GPIOB->CRH|=0X00030000;
  GPIOB->ODR|=1<<12;    	//PB12���� ��ֹW25Qxx�ĸ��� 
  
  RCC->APB2ENR|=1<<8;		//ʹ��PORTGʱ�� 
  GPIOG->CRL&=0X00FFFFFF;       //PG6��/��������,PG7�������
  GPIOG->CRL|=0X38000000;	
  GPIOG->CRH&=0XFFFFFFF0;
  GPIOG->CRH|=0X00000003;	//PG8 ������� 	    
  GPIOG->ODR|=7<<6;     	//PG6 7 8 ����	
 */  
  
   SPI2_init();          //����SPI2
   
  SPI2->CR1&=~(1<<6); 	//SPI�豸ʧ��
  SPI2->CR1&=~(1<<1); 	//����ģʽ��SCKΪ0 CPOL=0
  SPI2->CR1&=~(1<<0); 	//���ݲ����ӵ�1��ʱ����ؿ�ʼ,CPHA=0  
  SPI2->CR1|=1<<6; 		//SPI�豸ʹ��
  
  NRF24L01_CE=0; 			//ʹ��24L01
  NRF24L01_CSN=1;			//SPIƬѡȡ��
  //delay_ms(200);
  
  /*
     //�����2401
  
  
  GPIOG->CRL&=0X00FFFFFF;       //PG6��/��������,PG7�������
  GPIOG->CRL|=0X38000000;	
  GPIOG->CRH&=0XFFFFFFF0;
  GPIOG->CRH|=0X00000003;	//PG8 ������� 	    
  GPIOG->ODR|=7<<6;     	//PG6 7 8 ����	 
  
  
  SPI2_init();                   //����SPI2
  
  //���NRF���ص��޸�SPI������
  SPI2->CR1&=~(1<<6); 	//SPI�豸ʧ��
  SPI2->CR1&=~(1<<1); 	//����ģʽ��SCKΪ0 CPOL=0
  SPI2->CR1&=~(1<<0); 	//���ݲ����ӵ�1��ʱ����ؿ�ʼ,CPHA=0  
  SPI2->CR1|=1<<6; 		//SPI�豸ʹ��
 
  NRF24L01_M_CE=0; 			//ʹ��24L01
  NRF24L01_M_CSN=1;			//SPIƬѡȡ��	
  */
  
}

void LED_init()    
{      
  RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��	   	 
  RCC->APB2ENR|=1<<6;    //ʹ��PORTEʱ��	
                   
  GPIOB->CRL&=0XFF0FFFFF; 
  GPIOB->CRL|=0X00300000;//PB.5 �������   	 
  GPIOB->ODR|=1<<5;      //PB.5 �����
                                                                                            
  GPIOE->CRL&=0XFF0FFFFF;
  GPIOE->CRL|=0X00300000;//PE.5�������
  GPIOE->ODR|=1<<5;      //PE.5����� 
  
}



void EXIT_init()
{
  
   Ex_NVIC_Config(GPIO_B,10,FTIR); 	//B.10  �ⲿ�ж� �½��ش���
   Ex_NVIC_Config(GPIO_B,11,FTIR); 	//B.10  �ⲿ�ж� �½��ش���
   Ex_NVIC_Config(GPIO_B,8,FTIR); 	//B.10  �ⲿ�ж� �½��ش���
   Ex_NVIC_Config(GPIO_B,9,FTIR); 	//B.10  �ⲿ�ж� �½��ش���
   
   MY_NVIC_Init(1,3,EXTI15_10_IRQn,2);//��ռ1�������ȼ�3����2	
  MY_NVIC_Init(1,3,EXTI9_5_IRQn,2);//��ռ1�������ȼ�3����2
   
}


void key_init()
{
  RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��
  GPIOB->CRH&=0XFFFF00FF; 
  GPIOB->CRH|=0X00008800;//PB.10 11 ����   	 
  GPIOB->ODR|=1<<10;    	//PB10 ����
  GPIOB->ODR|=1<<11;    	//PB11
  
  
  GPIOB->CRH&=0XFFFFFF00; 
  GPIOB->CRH|=0X00000088;//PB.8 9 �������   	 
  GPIOB->ODR|=1<<9;      //PB.9 �����
  GPIOB->ODR|=1<<8;      //PB.9 �����
  
  
}

void sys_config()
{
  //Stm32_Clock_Init(9);
  //SystemInit();
  delay_init();
  
  //delay_ms(4000);  //delay 4s
  
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�    
  
  //uart_init(230400);   //115200
  //RCC->APB2ENR&=0xFFFFFBFF;    //�ص�uart1ʱ��
  
  key_init();
  
  n2401_init();
  NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
  NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
  while(NRF24L01_Check());
  
  //NRF24L01_RX_Mode();//��������
  
  //MPU_Init();
 // HMC5883_init();
  //LED_init();
  
  SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��
 
  
  Adc_Init();
  OLED_Init();
  
  OLED_ShowString(0,0,"XPH_UAV");  //NAME
  delay_ms(500);

  oled_offset();
   
  EXIT_init();
  
  
  TIM3_init(19,7199);  //6ms
  
}











