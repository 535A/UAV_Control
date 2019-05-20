#ifndef SYS_INIT_H
#define SYS_INIT_H

void sys_config();
void TIM3_init(u16 arr,u16 psc);
void TIM1_PWM_init(u16 arr,u16 psc);

#define COUNT PBout(9)	
//#define NRFCOUNT PBout(3)	
#endif