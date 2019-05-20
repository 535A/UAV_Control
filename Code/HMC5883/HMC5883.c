/***************************** (C) COPYRIGHT NULL С�ƺ� *************************

 * �ļ���       ��HMC5883
 * ����         ��
 *
 * ����         ��С�ƺ�
 * ʱ��         ��2016.12.1

**********************************************************************************/

#include "com_def.h"
   

extern double mx,my,mz;   
extern double mx_max,my_max,mz_max;
extern double mx_min,my_min,mz_min;
extern double mxo,myo,mzo;

u8 HMC5883_init()
{
  
  /*
    HMC_Write_Byte(0x00,0x78);  //75Hz  ������ƽ����8��
    //HMC_Write_Byte(0x01,0xe0);  //75Hz  ������ƽ����8��
    HMC_Write_Byte(0x02,0x00);
  */
  
  MPU_IIC_Start();
  MPU_IIC_Send_Byte(0x3c);
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }

  MPU_IIC_Send_Byte(0x00);
  if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Send_Byte(0x14);
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Stop();
  
  
  MPU_IIC_Start();
  MPU_IIC_Send_Byte(0x3c);
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Send_Byte(0x01);
  if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Send_Byte(0x40);
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Stop();
   
  
  
  MPU_IIC_Start();
  MPU_IIC_Send_Byte(0x3c);
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Send_Byte(0x02);
  if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Send_Byte(0x00);
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
    
    
  MPU_IIC_Stop();
  return 0;	
}

//��ȡHMC5883��ֵ
u8 HMC_read()
{
  //u8 i=0;
  static u16 XYZ_Data[6]; //�����洢�����������������
  
  
  
  //HMC_Write_Byte(0x03,0x3d); // ����HMC5883L��������ַ0x3c��д����
  MPU_IIC_Start(); 
  //delay_ms(10);
  MPU_IIC_Send_Byte(0x3c);
  if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Send_Byte(0x03);
  if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  MPU_IIC_Start(); 
  MPU_IIC_Send_Byte(0x3d);
  if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
      MPU_IIC_Stop();		 
      return 1;		
    }
  
  //���ζ�ȡ�������������
  XYZ_Data[0]=MPU_IIC_Read_Byte(1);
  XYZ_Data[1]=MPU_IIC_Read_Byte(1);
  XYZ_Data[2]=MPU_IIC_Read_Byte(1);
  XYZ_Data[3]=MPU_IIC_Read_Byte(1);
  XYZ_Data[4]=MPU_IIC_Read_Byte(1);
  XYZ_Data[5]=MPU_IIC_Read_Byte(0);
  
  MPU_IIC_Stop();
  
  
  mx=(double)((int16_t)((XYZ_Data[0]<<8)+XYZ_Data[1]));
  mz=(double)((int16_t)((XYZ_Data[2]<<8)+XYZ_Data[3]));
  my=(double)((int16_t)((XYZ_Data[4]<<8)+XYZ_Data[5]));
  
  
  if(mx>mx_max) mx_max=mx;
  if(my>my_max) my_max=my;
  if(mz>mx_max) mz_max=mz;
  
  if(mx<mx_min) mx_min=mx;
  if(my<my_min) my_min=my;
  if(mz<mx_min) mz_min=mz;
  
  mxo=(mx_max+mx_min)/2;
  myo=(my_max+my_min)/2;
  mzo=(mz_max+mz_min)/2;
  
  mx=(mx-mxo)*0.000001012001648;
  my=(my-myo)*0.000001012001648;
  mz=(mz-mzo)*0.000001012001648;
  
  
  
  return 0;	
}




