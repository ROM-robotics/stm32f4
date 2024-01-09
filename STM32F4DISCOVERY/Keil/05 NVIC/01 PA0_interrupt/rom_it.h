/*******************************************************************************
 *                    ROM ROBOTICS Co,.Ltd ( Myanmar )    										 *
 *			(?) COPYLEFT 2023		| www.romrobots.com | (+95) 9-259-288-229 				 *
 *******************************************************************************/ 
#ifndef __ROM_IT_H
#define __ROM_IT_H

#include "variables.h"

#define __ROM_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

void EXTI0_IRQHandler(void);

#endif
