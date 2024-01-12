/*******************************************************************************
 *                    ROM ROBOTICS Co,.Ltd ( Myanmar )    										 *
 *			(?) COPYLEFT 2023		| www.romrobots.com | (+95) 9-259-288-229 				 *
 *******************************************************************************/ 
#ifndef __ROM_CLOCK_H
#define __ROM_CLOCK_H

#include "variables.h"

void ___ROM_SystemClock_Config(_Bool use_pc9_clock_output);
void ROM_USE_PLL_CLOCK(_Bool clock_output);

void ROM_CLOCK_MEASURE_FROM_PC9_MCO2(void);
void ROM_CLOCK_MEASURE_FROM_PC9_GPIO(void);


#endif
