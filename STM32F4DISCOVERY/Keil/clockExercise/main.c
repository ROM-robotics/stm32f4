#include "stm32f4xx.h"


int main()
{
	// enable adc1 clock
	RCC->APB2ENR |= 1 << 8;
	ADC_TypeDef *pADC;
	pADC = ADC1;
	
	pADC->CR1 = 0x55;
	
	
	return 0;
}
