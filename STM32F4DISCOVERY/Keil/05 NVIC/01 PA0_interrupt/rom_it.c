#include "rom_it.h"

void EXTI0_IRQHandler(void) 
{
    // Your code here
	
    // Clear the EXTI line pending bit
	  __ROM_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	
}
