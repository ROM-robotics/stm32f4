#include "stm32f4xx.h"

int main()
{
	// 1. turn on HSE Oscillator
	RCC->CR |= ( 1 << 16);
	// 2. wait until HSE becomes stable.
	while ( !(RCC->CR & ( 1 <<17 )) );
	
	// 3 select HSE  as sysclk
	RCC->CFGR &= ~( 0x3 << 0 );
	RCC->CFGR |= 1;
	
	
	
}
