#include "stm32f4xx_hal.h"

// Function prototypes
void SystemClock_Config(void);
void Error_Handler(void);
void TIM2_Init(void);
void TIM2_IRQHandler(void);

TIM_HandleTypeDef htim2;
int main(void) {
    // HAL initialization
    HAL_Init();

    // System Clock Configuration
    SystemClock_Config();

    // Timer 2 Initialization
    TIM2_Init();

    // Enable the peripheral clock for GPIO Port D
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // Configure GPIO Pin D13 as output
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    // Start Timer 2
    HAL_TIM_Base_Start_IT(&htim2);

    while (1) {
        // Main loop, do other tasks here
    }
}

void TIM2_Init(void) {
    // Enable the peripheral clock for Timer 2
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Timer 2 Configuration
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 8400 - 1; // Assuming a 84MHz clock, this gives a 10kHz frequency
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 10000 - 1;   // 1s period with a 10kHz frequency
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }

    // Enable Timer 2 interrupt
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

// Timer 2 interrupt handler
void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

// Callback function for Timer 2 interrupt
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // Toggle LED on GPIO Pin D13
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
    }
}

void Error_Handler(void) {
    while (1) {
        // Error occurred, stay in this loop
    }
}

// System Clock Configuration
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure the main internal regulator output voltage
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Initializes the CPU, AHB and APB buses clocks
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    // Initializes the CPU, AHB and APB buses clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}
