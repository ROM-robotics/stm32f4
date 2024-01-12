#include "rom_it.h"
void EXTI0_IRQHandler(void) 
{
    // Your code here
	
    // Clear the EXTI line pending bit
	  __ROM_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if( TIM9->SR & TIM_SR_UIF )
	{
		green_led = ~green_led;
	}
	TIM9->SR &= ~ TIM_SR_UIF;
}
