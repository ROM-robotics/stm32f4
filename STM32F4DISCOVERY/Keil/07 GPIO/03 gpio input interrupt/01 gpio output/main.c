#include "stm32f4xx_hal.h"

void configLEDPins(void);
void msDelay(uint32_t ms);

int main(void)
{
	configLEDPins();
	
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
		msDelay(1000);
	}
	
}

void configLEDPins(void)
{
	GPIO_InitTypeDef my_leds;
	my_leds.Mode = GPIO_MODE_OUTPUT_PP;
	my_leds.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	
	HAL_GPIO_Init(GPIOD, &my_leds);
	
}

void msDelay(uint32_t ms)
{
	for(uint32_t i=0;i<ms*4000;i++);
}

