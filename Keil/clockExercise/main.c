#include "stm32f4xx.h"


int main()
{
	
	ADC_TypeDef *pADC;
	pADC = ADC1;
	
	pADC->CR1 = 0x55;
	
	return 0;
}
