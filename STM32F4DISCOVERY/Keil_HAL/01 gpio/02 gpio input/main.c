#include "stm32f4xx_hal.h"

void configLEDPins(void);
void msDelay(uint32_t ms);

int main(void)
{
	configLEDPins();
	GPIO_PinState btn_state;
	
	while(1)
	{
		btn_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, btn_state);
		msDelay(100);
	}
	
}

void configLEDPins(void)
{
	// clock 
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef my_leds;
	my_leds.Mode = GPIO_MODE_OUTPUT_PP;
	my_leds.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	
	HAL_GPIO_Init(GPIOD, &my_leds);
	
  // user btn
	my_leds.Mode = GPIO_MODE_INPUT;
	my_leds.Pin = GPIO_PIN_0;
	my_leds.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOA, &my_leds);
	
}

void msDelay(uint32_t ms)
{
	for(uint32_t i=0;i<ms*4000;i++);
}

