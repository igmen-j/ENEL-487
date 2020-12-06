/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "./usr/CLI.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FSM_MODE 			1000
#define SCM_MODE 			2000
#define ATM					100

#define FSM_ON 				1500
#define FSM_OFF 			500

#define PRI_GREEN_TIME 		83500
#define PRI_YELLOW_TIME		3500
#define SEC_GREEN_TIME		28000
#define SEC_YELLOW_TIME		3500
#define ALL_RED_TIME		6000

#define PRI_WALK_TIME		70500
#define	PRI_WALK_WARN_TIME	13000
#define	PRI_DONT_WALK_TIME	41000
#define SEC_WALK_TIME		93000
#define	SEC_WALK_WARN_TIME	11500
#define	SEC_DONT_WALK_TIME	20000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* Definitions for StateController */
osThreadId_t StateControllerHandle;
const osThreadAttr_t StateController_attributes = {
  .name = "StateController",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 128 * 4
};
/* Definitions for CLI */
osThreadId_t CLIHandle;
const osThreadAttr_t CLI_attributes = {
  .name = "CLI",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for Status_Update */
osThreadId_t Status_UpdateHandle;
const osThreadAttr_t Status_Update_attributes = {
  .name = "Status_Update",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for CLI_Queue */
osMessageQueueId_t CLI_QueueHandle;
const osMessageQueueAttr_t CLI_Queue_attributes = {
  .name = "CLI_Queue"
};
/* Definitions for Status_Queue */
osMessageQueueId_t Status_QueueHandle;
const osMessageQueueAttr_t Status_Queue_attributes = {
  .name = "Status_Queue"
};
/* USER CODE BEGIN PV */
uint8_t cliBufferTX[MAX_USER_INPUT];
uint8_t cliRXChar;
uint8_t cliBufferRX[MAX_USER_INPUT];
bool isCompleteLine = false;
int counter = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void State_Controller_Task(void *argument);
void CLI_Task(void *argument);
void Status_Update_Task(void *argument);

/* USER CODE BEGIN PFP */

void turnOffAllLights();

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
void StartBlink01(void *argument);
void RX_CLI_Task(void *argument);
void Status_CLI_Task(void *argument);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	cmd_counter = 0;
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  turnOffAllLights();

  //Clear the screen
  strcpy ( cliBufferTX, "\x1b[2J" );


  printStringBlocking("\x1b[2J");
  printStringBlocking(CLEAR_SCREEN);   // clear entire screen
  HAL_Delay(500);
  //Print the welcome message
  welcomeMessage();
  HAL_Delay(2000);

  //Clear the screen
  printStringBlocking(CLEAR_SCREEN);   // clear entire screen
  printStringBlocking("\x1b[5;1H");
  //printStringBlocking(GOTO_RC(5,1)); // cursor to row#, col#
//  char * escape = GOTO_RC(5,1);
 // int val = &escape;

  printStringBlocking("\x1b[5;r");
//  printStringBlocking(SET_SCROLL_ROW_TO_BOTTOM(5)); // set scrolling region
                                               // to row#->bottom
  printStringBlocking("\x1b[5;1H");
  //printStringBlocking(GOTO_RC(5,1)); // cursor to row#, col# (must call this
     	                                 // again, because the set scroll
  printStringBlocking(PROMPT);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of CLI_Queue */
  CLI_QueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &CLI_Queue_attributes);

  /* creation of Status_Queue */
  Status_QueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &Status_Queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of StateController */
  StateControllerHandle = osThreadNew(State_Controller_Task, NULL, &StateController_attributes);

  /* creation of CLI */
  CLIHandle = osThreadNew(CLI_Task, NULL, &CLI_attributes);

  /* creation of Status_Update */
  Status_UpdateHandle = osThreadNew(Status_Update_Task, NULL, &Status_Update_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
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
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|Red1_Pin|Yellow1_Pin|Green1_Pin
                          |Blue1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Red2_Pin|Yellow2_Pin|Green2_Pin|Blue2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 Red1_Pin Yellow1_Pin Green1_Pin
                           Blue1_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5|Red1_Pin|Yellow1_Pin|Green1_Pin
                          |Blue1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Red2_Pin Yellow2_Pin Green2_Pin Blue2_Pin */
  GPIO_InitStruct.Pin = Red2_Pin|Yellow2_Pin|Green2_Pin|Blue2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void turnOffAllLights() {
	HAL_GPIO_WritePin(GPIOA, Red1_Pin|Yellow1_Pin|Green1_Pin|Blue1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Red2_Pin|Yellow2_Pin|Green2_Pin|Blue2_Pin, GPIO_PIN_SET);
}

void printString (const char* message)
{
    int i=0;
    HAL_StatusTypeDef	st;

    //Have to wait for the last transmission to go.
    while(huart3.gState == HAL_UART_STATE_BUSY_TX){}

	for (const char* p = message; *p; ++p) {
    	cliBufferTX[i] =  *p;
    	i++;
    }
	st = HAL_UART_Transmit_IT(&huart3, cliBufferTX, i);
	if (st != HAL_OK)
	{
		Error_Handler();
	}

}

void printStringBlocking (const char* message)
{
    int i=0;
    HAL_StatusTypeDef	st;

    //Have to wait for the last transmission to go.
    while(huart3.gState == HAL_UART_STATE_BUSY_TX){}

	for (const char* p = message; *p; ++p) {
    	cliBufferTX[i] =  *p;
    	i++;
    }
	st = HAL_UART_Transmit(&huart3, cliBufferTX, i, 1000);
	if (st != HAL_OK)
	{
		Error_Handler();
	}

}

/*
 * Send a byte
 */
void sendByte (char msgChar)
{
    while(huart3.gState == HAL_UART_STATE_BUSY_TX){}
	HAL_UART_Transmit(&huart3, &msgChar, 1,1000);

}

void handleCommand(char command[]){
	uint8_t toggle[] = "toggle\r";
	uint8_t help[] = "help\r";
	uint8_t state[] = "state\r";

	if  (strcmp((char *) command, (char *) toggle) == 0) {
		printStringBlocking("\r\nToggle light\r\n");
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);	//turns on the light
	}
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_State_Controller_Task */
/**
  * @brief  Function implementing the StateController thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_State_Controller_Task */
void State_Controller_Task(void *argument)
{
  /* USER CODE BEGIN 5 */
  uint16_t cliMessage;
  uint16_t statusMessage;
  osStatus_t status;
  uint16_t state = SCM_MODE;
  uint16_t period = 10;

  //Send a status message straight away
  statusMessage = state;
  if(osMessageQueuePut(Status_QueueHandle, &statusMessage, 1U, 0U)!= osOK)
  {
    Error_Handler();
  }
  /* Infinite loop */
  for(;;)
  {

	//check for messages but do not block.
	status = osMessageQueueGet(CLI_QueueHandle, &cliMessage, NULL, 0U );
	if(status == osOK)
	{
		//This means a message has been received
		if (cliMessage <= ATM) {
			period = cliMessage;
		}

		if (cliMessage == FSM_MODE || cliMessage == FSM_MODE){
			state = cliMessage;
		}

		statusMessage = cliMessage;

		if(osMessageQueuePut(Status_QueueHandle, &statusMessage, 1U, 0U)!= osOK)
	    {
		  Error_Handler();
		}

	}

	if (state == FSM_MODE){
		HAL_GPIO_TogglePin(GPIOA, Red1_Pin);
		HAL_GPIO_TogglePin(GPIOB, Red2_Pin);
		osDelay(FSM_ON * period);
		HAL_GPIO_TogglePin(GPIOA, Red1_Pin);
		HAL_GPIO_TogglePin(GPIOB, Red2_Pin);
		osDelay(FSM_OFF * period);
	}
	else if (state == SCM_MODE){	//reset is on set is off
		turnOffAllLights();
		HAL_GPIO_WritePin(GPIOA, Red1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Red2_Pin, GPIO_PIN_RESET);
		osDelay(ALL_RED_TIME / period);

		HAL_GPIO_WritePin(GPIOA, Red1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, Green1_Pin, GPIO_PIN_RESET);
		osDelay(PRI_GREEN_TIME / period);

		HAL_GPIO_WritePin(GPIOA, Green1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, Yellow1_Pin, GPIO_PIN_RESET);
		osDelay(PRI_YELLOW_TIME / period);

		HAL_GPIO_WritePin(GPIOA, Yellow1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, Red1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Red2_Pin, GPIO_PIN_RESET);
		osDelay(ALL_RED_TIME / period);

		//secondary
		HAL_GPIO_WritePin(GPIOB, Red2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, Green2_Pin, GPIO_PIN_RESET);
		osDelay(SEC_GREEN_TIME / period);

		HAL_GPIO_WritePin(GPIOB, Green2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, Yellow2_Pin, GPIO_PIN_RESET);
		osDelay(SEC_YELLOW_TIME / period);

		HAL_GPIO_WritePin(GPIOB, Yellow2_Pin, GPIO_PIN_SET);
	}
  }

  osThreadTerminate(NULL);
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_CLI_Task */
/**
* @brief Function implementing the CLI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CLI_Task */
void CLI_Task(void *argument)
{
  /* USER CODE BEGIN CLI_Task */
	uint16_t cliMessage;
  /* Infinite loop */
	for(;;)
	{
		//This starts the process of receiving the data.
		if ((HAL_UART_Receive ( &huart3, &cliRXChar, 1, 10) == HAL_OK))
		{
			//Check if this is a complete line.
			isCompleteLine =  returnLineOrPartialLine (cliBufferRX, cliRXChar);

			if(isCompleteLine)
			{
				handleCommand(cliBufferRX); //Empty for now.

				//Assume that things are fine :)
				printStringBlocking("\r\n");
				printStringBlocking(PROMPT);

				//Send a message to the controller.

				//cliMessage -=100;
				if(osMessageQueuePut(CLI_QueueHandle, &cliMessage, 1U, 0U)!= osOK)
				{
				  Error_Handler();
				}
			}
		}
		osDelay(50);

  }
  /* USER CODE END CLI_Task */
}

/* USER CODE BEGIN Header_Status_Update_Task */
/**
* @brief Function implementing the Status_Update thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Status_Update_Task */
void Status_Update_Task(void *argument)
{
  /* USER CODE BEGIN Status_Update_Task */

	uint16_t statusMessage;
	uint16_t period;
	osStatus_t status;

	/* Infinite loop */
	for(;;)
	{
		status = osMessageQueueGet(Status_QueueHandle, &statusMessage, NULL, 0U );
		if(status == osOK)
		{

			period = statusMessage;
		}
		//For some damn reason it locks if I send this twice in a row.
		//	static const int32_t STR_SIZE = 96;
		static char outstring[50];

		printStringBlocking(SAVE_CURSOR);
		printStringBlocking("\x1b[1;1H");
		//printStringBlocking(GOTO_RC(1,1));

		//GOTO_RC(1,1);
		snprintf(outstring, 50,"period:% 5d \r\n",period);
		printStringBlocking(outstring);
		printStringBlocking(RESTORE_CURSOR);

		osDelay(1000);
	}
  /* USER CODE END Status_Update_Task */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
