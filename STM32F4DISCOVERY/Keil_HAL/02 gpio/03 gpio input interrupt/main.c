#include "stm32f4xx_hal.h"

void configLEDPins(void);
void msDelay(uint32_t ms);

int main(void)
{
	configLEDPins();
	
	
	while(1)
	{
		
	}
	
}

void configLEDPins(void)
{
	// clock 
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef my_leds;
	my_leds.Mode = GPIO_MODE_OUTPUT_PP;
	my_leds.Pin = GPIO_PIN_12;
	
	HAL_GPIO_Init(GPIOD, &my_leds);
	
  // user btn
	my_leds.Mode = GPIO_MODE_IT_RISING;
	my_leds.Pin = GPIO_PIN_0;
	
	HAL_GPIO_Init(GPIOA, &my_leds);
	
	// NVIC
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0); // priority 0 , sub priority 0;
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	// we need to enable systick interrupt
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
	
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	for(uint32_t i=0;i<10000;i++);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}


void msDelay(uint32_t ms)
{
	for(uint32_t i=0;i<ms*4000;i++);
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

