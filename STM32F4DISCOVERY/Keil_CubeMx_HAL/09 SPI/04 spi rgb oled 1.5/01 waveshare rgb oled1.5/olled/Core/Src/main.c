/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1351.h"
#include "fonts.h"
#include "testimg.h"
#include <string.h>
#include <stdarg.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t x_offset = 16;
void loop(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

    //SSD1351_Unselect();
	SSD1351_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 // SSD1351_FillScreen(SSD1351_MAGENTA);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  loop();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void loop(void) {
    // Check border
    SSD1351_FillScreen(SSD1351_BLACK);

    for(int x = 0; x < SSD1351_WIDTH; x++) {
        SSD1351_DrawPixel(x, 0, SSD1351_RED);
        SSD1351_DrawPixel(x, SSD1351_HEIGHT-1, SSD1351_RED);
    }

    for(int y = 0; y < SSD1351_HEIGHT; y++) {
        SSD1351_DrawPixel(0, y, SSD1351_RED);
        SSD1351_DrawPixel(SSD1351_WIDTH-1, y, SSD1351_RED);
    }

    HAL_Delay(3000);


    // Check fonts
    SSD1351_FillScreen(SSD1351_BLACK);
    SSD1351_WriteString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, SSD1351_RED, SSD1351_BLACK);
    SSD1351_WriteString(0, 3*10, "Font_11x18, green, lorem ipsum", Font_11x18, SSD1351_GREEN, SSD1351_BLACK);
    SSD1351_WriteString(0, 3*10+3*18, "Font_16x26, blue, lorem ipsum dolor sit amet", Font_16x26, SSD1351_BLUE, SSD1351_BLACK);

    HAL_Delay(1000);
    SSD1351_InvertColors(true);
    HAL_Delay(1000);
    SSD1351_InvertColors(false);

    HAL_Delay(5000);

    // Check colors
    SSD1351_FillScreen(SSD1351_WHITE);
    SSD1351_WriteString(0, 0, "WHITE", Font_11x18, SSD1351_BLACK, SSD1351_WHITE);
    HAL_Delay(500);

    SSD1351_FillScreen(SSD1351_BLUE);
    SSD1351_WriteString(0, 0, "BLUE", Font_11x18, SSD1351_BLACK, SSD1351_BLUE);
    HAL_Delay(500);

    SSD1351_FillScreen(SSD1351_RED);
    SSD1351_WriteString(0, 0, "RED", Font_11x18, SSD1351_BLACK, SSD1351_RED);
    HAL_Delay(500);

    SSD1351_FillScreen(SSD1351_GREEN);
    SSD1351_WriteString(0, 0, "GREEN", Font_11x18, SSD1351_BLACK, SSD1351_GREEN);
    HAL_Delay(500);

    SSD1351_FillScreen(SSD1351_CYAN);
    SSD1351_WriteString(0, 0, "CYAN", Font_11x18, SSD1351_BLACK, SSD1351_CYAN);
    HAL_Delay(500);

    SSD1351_FillScreen(SSD1351_MAGENTA);
    SSD1351_WriteString(0, 0, "MAGENTA", Font_11x18, SSD1351_BLACK, SSD1351_MAGENTA);
    HAL_Delay(500);

    SSD1351_FillScreen(SSD1351_YELLOW);
    SSD1351_WriteString(0, 0, "YELLOW", Font_11x18, SSD1351_BLACK, SSD1351_YELLOW);
    HAL_Delay(500);

    SSD1351_FillScreen(SSD1351_BLACK);
    SSD1351_WriteString(0, 0, "BLACK", Font_11x18, SSD1351_WHITE, SSD1351_BLACK);
    HAL_Delay(500);

    SSD1351_DrawImage(0, 0, 128, 128, (const uint16_t*)test_img_128x128);

    HAL_Delay(10000);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
