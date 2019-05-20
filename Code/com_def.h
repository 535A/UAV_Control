/***************************** (C) COPYRIGHT NULL С�ƺ� *************************

 * �ļ���       ��
 * ����         ��
 *
 * ����         ��С�ƺ�
 * ʱ��         ��2016.08.17

**********************************************************************************/

//����ͷ�ļ�����   #include "com_def.h"

#include "delay.h"
#include "spi.h"
#include "usart.h"
#include "24l01.h"
#include "sys_init.h"
#include "mpu6050.h"
#include "sys.h"
//#include "inv_mpu.h"
#include "mpuiic.h"
//#include "HMC5883.h"
#include "calculation.h"
#include "transfer.h"
#include <math.h>
#include "oled.h"
#include "adc.h"


//����
extern float pitch,roll,yaw;
extern float q0, q1, q2, q3;
extern float anglex,angley,anglez;  //�˲�֮��
extern int16_t moto1,moto2,moto3,moto4;
extern double mx,my,mz;   
extern double mx_max,my_max,mz_max;
extern double mx_min,my_min,mz_min;
extern double mxo,myo,mzo;
extern u16 ad[4];

extern u16 Control_ROLL,Control_PITCH,Control_YAW,R_THROTTLE,start,correct,UAV_OK;   //ң��  ƫ�� ���� ����
extern u8 RX_buf[9];
extern u8 TX_buf[9];  //��ѹ  ŷ����  ����  0.0 û�ˣ�

extern short gyro_x,gyro_y,gyro_z;
extern short angle_x,angle_y,angle_z;
extern float pitch0,roll0,yaw0;
extern float Hx,Hy,Alpha;   //Hx Hy �������mx my  Alpha�شŲ���
extern u8 canshu,jiesuo,oled_enter,oled_ex,f_cs;
extern int location;


extern PID PID_ROL,PID_PIT,PID_YAW;

//����
extern void run();
extern float abs_sz(float i);
extern void n2401_init();
extern void scan();


//һЩ�궨��
#define LED0 PBout(5)	// DS0
#define LED1 PEout(5)	// DS1	
#define key_up PBin(11)
#define key_down PBin(10)
#define key_en PBin(8)
#define key_ex PBin(9)
   
#define Angletorad    0.01745329252f  // �ȵ��Ƕ�
#define Radtoangle    57.295779513f   // ���ȵ��Ƕ�
#define Gyr_Gain 	    0.015267f       // ���ٶȱ�ɶ�                       
#define ACC_Gain 	    0.0011963f      // ���ٶȱ��G 
#define MAG_Gain	    0.1		//��ǿ��
   
#define Gyro_G 		0.0610351f