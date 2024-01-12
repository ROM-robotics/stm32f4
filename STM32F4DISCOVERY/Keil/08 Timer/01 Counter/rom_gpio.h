/*******************************************************************************
 *                    ROM ROBOTICS Co,.Ltd ( Myanmar )    										 *
 *			(?) COPYLEFT 2023		| www.romrobots.com | (+95) 9-259-288-229 				 *
 *******************************************************************************/ 
#ifndef __ROM_GPIO_H
#define __ROM_GPIO_H

#include "variables.h"


void ___ROM_GPIO_INIT(void);

void __gpio_leds_init(void);
	
// 0 --> falling edge, 1 --> rising edge, 2 --> both
//void __gpio_configure_interrupt(uint16_t pin_no, uint8_t edge);
//void __gpio_clear_interrupt(uint16_t pin_no);

// Emergency Stop
//void __gpio_emergency_stop(void);

#endif

/*
LD1 COM: LD1 default status is red. LD1 turns to green to indicate that communications are in progress between the PC and the ST-LINK/V2-A.

• LD2 PWR: red LED indicates that the board is powered.

• User LD3: orange LED is a user LED connected to the I/O PD13 of the STM32F407VGT6.

• User LD4: green LED is a user LED connected to the I/O PD12 of the STM32F407VGT6.

• User LD5: red LED is a user LED connected to the I/O PD14 of the STM32F407VGT6. 

• User LD6: blue LED is a user LED connected to the I/O PD15 of the STM32F407VGT6. 

• USB LD7: green LED indicates when VBUS is present on CN5 and is connected to PA9 of the STM32F407VGT6. 

• USB LD8: red LED indicates an over-current from VBUS of CN5 and is connected to the I/O PD5 of the STM32F407VGT6.

B1 USER: User and Wake-Up buttons are connected to the I/O PA0 of the STM32F407VG

*/
