#include "stm32f4xx_hal.h"

void TIM9_init(void);
TIM_HandleTypeDef my_timer9;

int main(void)
{
	TIM9_init();
	
	while(1)
	{
		
	}
}

void TIM9_init(void)
{
	TIM_ClockConfigTypeDef clock_source_config = {0};
	
	my_timer9.Instance = TIM9;
	my_timer9.Init.Prescaler = 15;
	my_timer9.Init.CounterMode = TIM_COUNTERMODE_UP;
	my_timer9.Init.Period = 10000;
	my_timer9.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;
	my_timer9.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	if( HAL_TIM_Base_Init(&my_timer9) != HAL_OK )
	{
		//Error_Handler();
	}
	
	clock_source_config.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	
	if( HAL_TIM_ConfigClockSource(&my_timer9, &clock_source_config) != HAL_OK )
	{
		//Error_Handler();
	}
	
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  // 10 ms
}
