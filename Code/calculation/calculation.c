#include "com_def.h"


void updatePID(float rol_P,float rol_I,float rol_D,float pit_P,float pit_I,float pit_D,float yaw_P,float yaw_I,float yaw_D)
{
 //由上位机给出
  
        PID_ROL.P = rol_P;
	PID_ROL.I = rol_I;
	PID_ROL.D = rol_D;
	
	PID_PIT.P = pit_P;
	PID_PIT.I = pit_I;
	PID_PIT.D = pit_D;
	
	PID_YAW.P = yaw_P;
	PID_YAW.I = yaw_I;
	PID_YAW.D = yaw_D;

}





void CONTROL()
{
       ;
}




/////////  第 二 套 /////////

PID PID_ROL,PID_PIT,PID_YAW;


u16 Motor1,Motor2,Motor3,Motor4;
void MOTO_PWMRFLASH(u16 motor1,u16 motor2,u16 motor3,u16 motor4)
{
  ;
}


