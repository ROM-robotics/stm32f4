#include "rom_gpio.h"


void ___ROM_GPIO_INIT(void) 
{
	__gpio_leds_init();
	//__gpio_emergency_stop();
} 
/*
void __gpio_emergency_stop(void)
{
	// GPIOD CLOCK ENABLE
	RCC->AHB1ENR = RCC_AHB1ENR_GPIOAEN;
	
	// input or output
	GPIOA->MODER &= ~(1u<<1 | 1u<<0);  // pa0 input mode
	
	GPIOA->PUPDR &= ~(1u<<1 | 1u<<0);  // no pull pu pull down
}
*/
void __gpio_leds_init(void)
{
	// GPIOD CLOCK ENABLE
	RCC->AHB1ENR = RCC_AHB1ENR_GPIODEN;
	
	// input or output
	GPIOD->MODER &= ~(3u<<24 & 3u<<26 & 3u<<28 & 3u<<30);
	GPIOD->MODER  |= 1 << 24 		// PD12 output
								| 1 << 26			// PD13
								| 1 << 28			// PD14
								| 1 << 30;		// PD15
	
	// what speed
	GPIOD->OSPEEDR &= ~(3u<<24 & 3u<<26 & 3u<<28 & 3u<<30);
	GPIOD->OSPEEDR |= 1 << 24 	// PD12 medium speed
								 | 1 << 26		// PD13
								 | 1 << 28		// PD14
								 | 1 << 30;		// PD15
}
/*
void __gpio_configure_interrupt(uint16_t pin_no, uint8_t edge)
{ // FOR EMERGENCY ? MAY BE USELESS ? MAY BE GPIO INPUT ONLY
	if( edge == 0 )
	{
		EXTI->FTSR |= 1<<pin_no;
	}
	else if ( edge == 1 )
	{
		EXTI->RTSR |= 1<<pin_no;
	}
	else if ( edge == 2 ) 
	{
		EXTI->FTSR |= 1<<pin_no;
		EXTI->RTSR |= 1<<pin_no;
	}
	else {
		// Error Handler
	}
	
	// Enable gpio Interrupt
	EXTI->IMR |= 1<<pin_no;
	
	switch(pin_no)
	{
		case 0: NVIC_EnableIRQ(EXTI0_IRQn); break;
		case 1: NVIC_EnableIRQ(EXTI1_IRQn); break; 
		case 2: NVIC_EnableIRQ(EXTI2_IRQn); break;
		case 3: NVIC_EnableIRQ(EXTI3_IRQn); break;
		case 4: NVIC_EnableIRQ(EXTI4_IRQn); break;
		
		case 5: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
		case 6: NVIC_EnableIRQ(EXTI9_5_IRQn); break; 
		case 7: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
		case 8: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
		case 9: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
		
		case 10: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
		case 11: NVIC_EnableIRQ(EXTI15_10_IRQn); break; 
		case 12: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
		case 13: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
		case 14: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
		case 15: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
		default: break;
	}
	// Create FUNCTION --> void EXTIxx_IRQHandler(void) {} in main.c
}

void __gpio_clear_interrupt(uint16_t pin_no)
{
	if(EXTI->PR & ( 1<<pin_no))
	{
		EXTI->PR |= 1<<pin_no;
	}
}
*/
