#include "rom_nvic.h"

void ___ROM_SysTick_Init(void)
{
	NVIC_SetPriorityGrouping(0x00000003U);
	
	if( SysTick_Config(SystemCoreClock / 1000U) == 0Ul ) { while(1) {} }
	
	uint32_t prioritygroup = 0x00U;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, 15U, 0U));
}

void ___ROM_PA0_EXTI_Init(void)
{
	// Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure PA0 as input with pull-up
    GPIOA->MODER &= ~GPIO_MODER_MODER0; // Clear bits
    //GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0; // Set as pull-up

    // Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Connect EXTI Line0 to PA0 pin
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;

    // Configure EXTI Line0
    EXTI->IMR |= EXTI_IMR_MR0; // Enable interrupt on EXTI Line0
    EXTI->RTSR |= EXTI_RTSR_TR0; // Trigger on rising edge (change to EXTI_FTSR_TR0 for falling edge)
    EXTI->FTSR &= ~EXTI_FTSR_TR0; // Clear falling edge trigger if needed

    // Enable and set EXTI Line0 interrupt to the lowest priority
    NVIC_SetPriority(EXTI0_IRQn, 0x0F);
    NVIC_EnableIRQ(EXTI0_IRQn);
}


