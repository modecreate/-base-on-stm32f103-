#ifndef __PWM_H
#define __PWM_H	 
#include "sys.h"

//#define LED0 PCout(13)

//void My_TIM2_Init(u16 arr,u16 psc);//≥ı ºªØ

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);

		 				    
#endif