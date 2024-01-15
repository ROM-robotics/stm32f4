#include "main.h"

CAN_HandleTypeDef hcan1;
TIM_HandleTypeDef htim9;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM9_Init(void);

void CAN1_Tx(void);
void CAN1_Rx(void);
void CAN1_FilterConfig(void);
void Led_manage(uint8_t led);
void SendResponse( uint32_t StdId );
uint8_t led_no = 0;
uint8_t request_counter = 0;

CAN_RxHeaderTypeDef RxHeader;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_TIM9_Init();
	
	CAN1_FilterConfig();
	
	// before CAN1 Start, Enable Interrupt
	if( HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK ) 
	{
		Error_Handler();
	} 
	// this need 11 continuous ressive state
	//if( HAL_CAN_Start(&hcan1) != HAL_OK ) { Error_Handler(); }
	
  while (1)
  {
  }
  
}

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
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
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


static void MX_CAN1_Init(void)
{
  hcan1.Instance = CAN1;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
	
	
  hcan1.Init.Prescaler = 3;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_11TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
	
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_TIM9_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 16799;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 9999;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim ) 
{
	CAN_TxHeaderTypeDef TxHeader;
	
	uint32_t mailbox;
	
	uint8_t msg; 
	
	if(request_counter == 4)
	{
		TxHeader.DLC = 2; // Node 1 demanding 2 Bytes of reply
		TxHeader.StdId = 0x651;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.RTR = CAN_RTR_REMOTE;
		if( HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &msg, &mailbox) != HAL_OK )
		{
			Error_Handler();
		}
		request_counter = 0;
	} else {
		CAN1_Tx();
		request_counter++;
	}
	
	UNUSED(htim);
}

/* USER CODE BEGIN 4 */
void CAN1_Tx(void)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;
	uint8_t msg;
	
	msg = ++led_no;
	if( led_no == 4 ) { led_no = 0; }
	
	TxHeader.DLC = 1;
	TxHeader.StdId = 0x65D;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	
	if( HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &msg, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
}



void CAN1_FilterConfig(void)
{
	CAN_FilterTypeDef can1_filter;
	
	can1_filter.FilterActivation = ENABLE;
	can1_filter.FilterBank = 0;
	can1_filter.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_filter.FilterIdHigh = 0x0000;
	can1_filter.FilterIdLow = 0x0000;
	can1_filter.FilterMaskIdHigh = 0x01C0;
	can1_filter.FilterMaskIdLow = 0x0000;
	can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter.FilterScale = CAN_FILTERSCALE_32BIT;

	if( HAL_CAN_ConfigFilter(&hcan1, &can1_filter) != HAL_OK )
	{
		Error_Handler();
	}
}

// Callback functions
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	// printf()
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	// printf()
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	// printf()
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	
	uint8_t receive_msg[8];
	
	if ( HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader, receive_msg ) != HAL_OK )
	{
		Error_Handler();
	}
	
	if( RxHeader.StdId == 0x65D && RxHeader.RTR == 0 )
	{
		// This is data frame sent by N1
		Led_manage(receive_msg[0]);
		// sprintf()
	}
	else if ( RxHeader.StdId == 0x651 && RxHeader.RTR == 1 )
	{
		// This is remote frame sent by N1
		SendResponse(RxHeader.StdId);
		return;
	}
	else if ( RxHeader.StdId == 0x651 && RxHeader.RTR == 0 )
	{
		// This is data frame sent by N2
		// sprintf()
	}
	
}
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	// printf();
}

void Led_manage(uint8_t led)
{
	switch( led )
	{
		case 1: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		
		case 2: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		
		case 3: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		
		case 4: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	}
}
void SendResponse( uint32_t StdId )
{
	CAN_TxHeaderTypeDef txHeader;

	uint32_t txMailbox;

	uint8_t response[2] = { 0xAB, 0xCD };

	txHeader.DLC = 2;
	txHeader.StdId = StdId;
	txHeader.IDE = CAN_ID_STD;
	txHeader.RTR = CAN_RTR_DATA;

	if( HAL_CAN_AddTxMessage(&hcan1, &txHeader, response, &txMailbox) != HAL_OK )
	{	Error_Handler();	}
}
/* USER CODE END 4 */


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
