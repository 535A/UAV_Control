#include "usart.h"	
#include "com_def.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ�����ʺ�STM32F10xϵ�У�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.7
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
//V1.6�޸�˵�� 20150109
//uart_init����ȥ���˿���PE�ж�
//V1.7�޸�˵�� 20150322
//�޸�OS_CRITICAL_METHOD���ж�Ϊ��SYSTEM_SUPPORT_OS
////////////////////////////////////////////////////////////////////////////////// 	  
extern void usart2_send_char(u8 c);
/////////////////////////////////////////////////////////////////  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB  
#if defined ( __ICCARM__ )  //IAR��ʹ��  
//�ض���fputc����   
int fputc(int ch, FILE *f)  
{        
    while((USART2->SR&0X40)==0);//ѭ������,ֱ���������     
    USART2->DR = (u8) ch;        
    return ch;  
}  
#else  //��IAR��ʹ��  
#pragma import(__use_no_semihosting)               
//��׼����Ҫ��֧�ֺ���                   
struct __FILE   
{   
    int handle;   
};   
  
FILE __stdout;         
//����_sys_exit()�Ա���ʹ�ð�����ģʽ      
_sys_exit(int x)   
{   
    x = x;   
}   
//�ض���fputc����   
int fputc(int ch, FILE *f)  
{        
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������     
    USART1->DR = (u8) ch;        
    return ch;  
}  
#endif  



#if EN_USART2_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void USART2_IRQHandler(void)
{
	u8 res;	

	if(USART2->SR&(1<<5))	//���յ�����
	{	 
		res=USART2->DR; 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
                                        ANO_DT_Data_Receive_Prepare(res);
				}		 
			}
		}
        
	}

} 
#endif										 
//��ʼ��IO ����2
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;
        u32 pclk2=72;
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
        mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0XFFFF00FF;//IO״̬����
	GPIOA->CRL|=0X00008B00;//IO״̬���� 
	RCC->APB1RSTR|=1<<17;   //��λ����1
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
#if EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,4,USART2_IRQn,3);//��2��������ȼ� 
#endif
}




//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart2_niming_report(u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	
	for(i=0;i<len;i++)send_buf[i]=data[i];			//��������
	
	for(i=0;i<len;i++)usart2_send_char(send_buf[i]);	//�������ݵ�����1 
}

/*
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart2_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart2_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
}  

*/
/*ͨ�����ڷ���PID��������λ��



*/
/*
void usart_report_pid(short rol_p, short rol_i, short rol_d, short pit_p, short pit_i, short pit_d, short yaw_p, short yaw_i, short yaw_d )
{
  u8 tbuf[28];
  u8 i;
  for(i=0;i<28;i++) tbuf[i]=0; //clear 
  tbuf[0]=0XAD;
  tbuf[1] = (rol_p>>8)&0xFF;
  tbuf[2] = rol_p&0xFF;
  tbuf[3] = (rol_i>>8)&0xFF;
  tbuf[4] = rol_i&0xFF;
  tbuf[5] = (rol_d>>8)&0xFF;
  tbuf[6] = rol_d&0xFF;
  tbuf[7] = (pit_p>>8)&0xFF;
  tbuf[8] = pit_p&0xFF;
  tbuf[9] = (pit_i>>8)&0xFF;
  tbuf[10] = pit_i&0xFF;
  tbuf[11] = (pit_d>>8)&0xFF;
  tbuf[12] = pit_d&0xFF;
  tbuf[13] = (yaw_p>>8)&0xFF;
  tbuf[14] = yaw_p&0xFF;
  tbuf[15] = (yaw_i>>8)&0xFF;
  tbuf[16] = yaw_i&0xFF;
  tbuf[17] = (yaw_d>>8)&0xFF;
  tbuf[18] = yaw_d&0xFF;
  usart2_niming_report(0XAC,tbuf,28);
    
}
*/
