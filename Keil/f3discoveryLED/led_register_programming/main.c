#include "stm32f3xx.h"

int main()
{
	// IOPE CLOCK ENABLE
	RCC->AHBENR |= 1<<21;
	
	// pin mode output
	GPIOE->MODER |= 1<<16 | 1<<18 | 1<<20 | 1<<22 | 1<<24 | 1<<26 | 1<<28 | 1<<30;
	
	// gpioe medium speed
	GPIOE->OSPEEDR |= 1<<16 | 1<<18 | 1<<20 | 1<<22 | 1<<24 | 1<<26 | 1<<28 | 1<<30;
	
	
	while(1)
	{
		GPIOE->ODR = 0x0000FF00;
		for(int i=0;i<100000;i++);
		GPIOE->ODR = 0x00000000;
		for(int i=0;i<100000;i++);
	}
}
