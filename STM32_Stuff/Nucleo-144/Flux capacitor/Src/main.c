/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define DEBUG_MODE 1


#define MAXSTRING 27

char	rxBuffer = '\000';
char	rxString[MAXSTRING];
int rxindex = 0;

#define BURST_TIME	70
#define READY_TIME	150
#define TIME_OUT	300
uint16_t ready, burst, first;
uint16_t timer;
volatile uint16_t breath_mode, click_count;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM4_Init(void);                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void print(char string[MAXSTRING]);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	char button_pressed  = 0;
	

	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_TIM14_Init();
  MX_TIM4_Init();

  /* USER CODE BEGIN 2 */
//	HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	
	
	GPIO_TypeDef *led_port[]={LD1_GPIO_Port, LD2_GPIO_Port, LD3_GPIO_Port};
	int led_pin[]={LD1_Pin, LD2_Pin ,LD3_Pin};

	char buff[30];
	
//	for(char i=0; i<3; i++){
//		for(char j=(i+1)*2;j>0;j--){
////			
////			sprintf(buff, "\r\n  i= %i; j= %i",i,j);
////			HAL_UART_Transmit(&huart3, (uint8_t*)buff, strlen(buff), 0xFFFF); 
////			
//		switch (i){
//			case 0: {	HAL_GPIO_TogglePin(led_port[0], led_pin[0]); 
//								break;}
//			case 1: {	HAL_GPIO_TogglePin(led_port[0], led_pin[0]); 
//								HAL_GPIO_TogglePin(led_port[1], led_pin[1]); 
//								break;}
//			case 2: {	HAL_GPIO_TogglePin(led_port[0], led_pin[0]); 
//								HAL_GPIO_TogglePin(led_port[1], led_pin[1]); 
//								HAL_GPIO_TogglePin(led_port[2], led_pin[2]); 
//								break;}
//		}
//		HAL_Delay(100);
//		}
//		HAL_Delay(100); 
//	}

		
//	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	
	char *msg = "\n\rNucleo-144 ISR-UART-DMA Fun!\n\r";

  HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 0xFFFF);
	HAL_UART_Receive_DMA(&huart3, &rxBuffer, 1);
	


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
		uint16_t dutyCycle = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2);
		sprintf(buff, "\r\ndutyCycle: %d", dutyCycle);
		HAL_UART_Transmit(&huart3, (uint8_t*)buff, strlen(buff), 0xFFFF);
	
  	while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	

		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		HAL_Delay(250);
		
		if(breath_mode == 0x01){		
				while(dutyCycle < __HAL_TIM_GET_AUTORELOAD(&htim4)) {
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, ++dutyCycle);					
					HAL_Delay(1);
				}

				while(dutyCycle > 0) {
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, --dutyCycle);
					HAL_Delay(1);
				}
		}
//		
//		
//		
//		if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin)){
//			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//			HAL_Delay(100);
//		}

//		
//		if(HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin)){		
//			char buff[30];
//			sprintf(buff, "\r\nUSER BUTTON status: %s; button_pressed: %i",
//			HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET ? "PRESSED" : "RELEASED"
//			,button_pressed);
//			HAL_UART_Transmit(&huart3, (uint8_t*)buff, strlen(buff), 0xFFFF); 
//			
//			if(button_pressed==0)
//			{
//				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//				button_pressed=1;
//				HAL_Delay(100);
//			}
//		
//		}
//			
//		else 
//			{
//			button_pressed=0;
//			HAL_Delay(100);
//			}
//		

  
	}
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 159;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim4);

}

/* TIM14 init function */
static void MX_TIM14_Init(void)
{

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 0;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 15999;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA8   ------> USB_OTG_FS_SOF
     PA9   ------> USB_OTG_FS_VBUS
     PA10   ------> USB_OTG_FS_ID
     PA11   ------> USB_OTG_FS_DM
     PA12   ------> USB_OTG_FS_DP
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

// ISR -------------------------------------------------------

// nereikia, jei CubeMX yra ijungta NVIC -> EXTI Line [0:15] interrupts

//void EXTI15_10_IRQHandler(void) {
//	HAL_GPIO_EXTI_IRQHandler(USER_Btn_Pin);
//	//HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
//}




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	
	if(GPIO_Pin == USER_Btn_Pin){
		char *msg = "\n\r --- User Button! ---\n\r";
		HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 0xFFFF); 
	
		if (!first)
				{
					HAL_TIM_Base_Start_IT(&htim14);		
					first = 1;
					
					#ifdef DEBUG_MODE
					print("First Pulse\n\r");
					#endif
				} else
				
		if (burst)
			{
				timer = 0;
				first = 0;
				burst = 0;

				HAL_TIM_Base_Stop_IT(&htim14);

				#ifdef DEBUG_MODE
				print("False Pulse\n\r");
				#endif
			} else {

				if (ready)
					{
						HAL_TIM_Base_Stop_IT(&htim14);
						timer = 0;
						first = 0;
						ready = 0;

						HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
						
						breath_mode = breath_mode ^ 0x01;
//						if (breath_mode != 0x01) {
							click_count = 0;
//						}
//						
						#ifdef DEBUG_MODE
						print("ON/OFF \n\r");
						#endif
					}				
			}
			
						if (breath_mode != 0x01 && !burst && !ready){
				click_count++;
							
				char buff[30];
				sprintf(buff, "\r\n breath_mode: %d   click_count: %d \r\n", breath_mode, click_count);
				HAL_UART_Transmit(&huart3, (uint8_t*)buff, strlen(buff), 0xFFFF); 
				
				switch (click_count){
					case 1: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 249); break;
					case 2: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 499); break;
					case 3: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 749); break;
					case 4: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 999); break;
					case 5: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0); click_count = 0; break;				
				}
				
			}
						
			
			
//  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	
//	char buff[30];
	 
//	 sprintf(buff, "\r\nUSER BUTTON status: %s",
//	 HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_RESET ? "PRESSED" : "RELEASED");
//	 HAL_UART_Transmit(&huart3, (uint8_t*)buff, strlen(buff), 0xFFFF); 
	 
	 
//	 sprintf(buff, "\r\nSkaicius: %.2f", 122.12);
//	 HAL_UART_Transmit(&huart3, (uint8_t*)buff, strlen(buff), 0xFFFF); 
	
	 
		}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	// This callback is automatically called by the HAL on the UEV event
	if(htim->Instance == TIM14){
//		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		
		timer++;
		if (timer == BURST_TIME)
			{
				#ifdef DEBUG_MODE
				print("Burst\n\r");
				#endif
				burst = 1;
				ready = 0;
			} else
		if (timer == READY_TIME)
			{
				burst = 0;
				ready = 1;
				#ifdef DEBUG_MODE
				print("Ready\n\r");
				#endif
			} else
		if (timer == TIME_OUT)
			{
				#ifdef DEBUG_MODE
				print("Time out\n\r");
				#endif
				timer = 0;
				burst = 0;
				ready = 0;
				first = 0;
				HAL_TIM_Base_Stop_IT(&htim14);
			}
	}
}


//USART -------------------------------------------------
void print(char string[MAXSTRING])
{
	HAL_UART_Transmit(&huart3, (uint8_t*)string, strlen(string),  0xFFFF);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	__HAL_UART_FLUSH_DRREGISTER(&huart3); // Clear the buffer to prevent overrun


  HAL_UART_Transmit(&huart3, (uint8_t*)&rxBuffer, 1, 0xFFFF); //echo


  if (rxBuffer == 8 || rxBuffer == 127) // If Backspace or del
    {
      print(" \b"); // "\b space \b" clears the terminal character. Remember we just echoced a \b so don't need another one here, just space and \b
      rxindex--;
      if (rxindex < 0) rxindex = 0;
    }

  else if (rxBuffer == '\n' || rxBuffer == '\r') // If Enter
    {
//      executeSerialCommand(rxString);
      rxString[rxindex] = 0;
      rxindex = 0;

      print("\n\r");
      print(rxString);

      for (int i = 0; i < MAXSTRING; i++) rxString[i] = 0; // Clear the string buffer
    }

  else
    {
      rxString[rxindex] = rxBuffer; // Add that character to the string
      rxindex++;
      if (rxindex > MAXSTRING) // User typing too much, we can't have commands that big
    {
      rxindex = 0;
      for (int i = 0; i < MAXSTRING; i++) rxString[i] = 0; // Clear the string buffer
    }
    }

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
