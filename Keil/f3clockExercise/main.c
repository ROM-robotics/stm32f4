#include "stm32f3xx.h"

int main()
{
	ADC_TypeDef *pADC;
	pADC = ADC1;
	
	pADC->CR = 0x55;
	
	return 0;
}