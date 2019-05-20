#ifndef _calculation_H
#define _calculation_H

typedef struct PID{float P,pout,I,iout,D,dout,IMAX,OUT;}PID;





void MOTO_PWMRFLASH(u16 motor1,u16 motor2,u16 motor3,u16 motor4);
void updatePID(float rol_P,float rol_I,float rol_D,float pit_P,float pit_I,float pit_D,float yaw_P,float yaw_I,float yaw_D);


#endif
