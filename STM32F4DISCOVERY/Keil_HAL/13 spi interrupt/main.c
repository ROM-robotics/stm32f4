#include "stm32f4xx_hal.h"

void GPIO_Config(void);
void SPI_Config(void);

SPI_HandleTypeDef my_spi;

uint8_t spi_tx_buf[2], spi_rx_buf[2];

int main(void)
{
	HAL_Init();
	GPIO_Config();
	SPI_Config();
	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spi_tx_buf[0] = 0x20;
	spi_tx_buf[1] = 0x91;
	HAL_SPI_Transmit(&my_spi, spi_tx_buf, 2, 10); // 10 ms wait
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	
	// Interrupt
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spi_tx_buf[0] = 0x23;
	spi_tx_buf[1] = 0xC8;
	HAL_SPI_Transmit(&my_spi, spi_tx_buf, 2, 10); // 10 ms wait
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	
	while(1)
	{
		
	}
}

void GPIO_Config(void)
{
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	GPIO_InitTypeDef my_pins;
	// SPI pins
	my_pins.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	my_pins.Mode = GPIO_MODE_AF_PP;
	my_pins.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	my_pins.Pull = GPIO_NOPULL;
	my_pins.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &my_pins);
	
	// Led
	my_pins.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	my_pins.Mode = GPIO_MODE_OUTPUT_PP;
	my_pins.Speed = GPIO_SPEED_FREQ_LOW;
	my_pins.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &my_pins);
	
	// CS 
	my_pins.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOE, &my_pins);
	
	// INT1
	my_pins.Pin = GPIO_PIN_0;
	my_pins.Mode = GPIO_MODE_IT_RISING;
	HAL_GPIO_Init(GPIOE, &my_pins);
	
	// Exti0
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void SPI_Config(void)
{
	my_spi.Instance = SPI1;
	my_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	my_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	my_spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	my_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	my_spi.Init.CRCPolynomial = 10;
	my_spi.Init.DataSize = SPI_DATASIZE_8BIT;
	my_spi.Init.Direction = SPI_DIRECTION_2LINES;
	my_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	my_spi.Init.Mode = SPI_MODE_MASTER;
	my_spi.Init.NSS = SPI_NSS_SOFT;
	my_spi.Init.TIMode = SPI_TIMODE_DISABLE;
	HAL_SPI_Init(&my_spi);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
