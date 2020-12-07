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


/*******************************************************************************
 *
 * Author: Justin Igmen
 * Class: ENEL 487
 * SID: 200364880
 *
 * This program simulates a traffic light controller.
 *
 * Credits: Trevor Douglas for providing part of the main.c file
 *
 *******************************************************************************/


/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "./usr/CLI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEFAULT_PERIOD		1

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

#define WALK_BLINK_TIME		1000
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
TIM_HandleTypeDef htim2;

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
uint16_t timer;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM2_Init(void);
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
  MX_TIM2_Init();
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
  printStringBlocking("\x1b[5;1H");		// cursor to row#, col#

  printStringBlocking("\x1b[5;r");	// set scrolling region
  printStringBlocking("\x1b[5;1H");// cursor to row#, col#
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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

/**
  * @brief Turns off all the LED to reset
  * @param None
  * @retval None
  */
void turnOffAllLights() {
	HAL_GPIO_WritePin(GPIOA, Red1_Pin|Yellow1_Pin|Green1_Pin|Blue1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Red2_Pin|Yellow2_Pin|Green2_Pin|Blue2_Pin, GPIO_PIN_SET);
}

/**
  * @brief Displays user's input to the CLI
  * @param message: pointer to user's message
  * @retval None
  */
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

/**
  * @brief Send a byte/char to the CLI
  * @param msgChar: user's char input
  * @retval None
  */
void sendByte (char msgChar)
{
    while(huart3.gState == HAL_UART_STATE_BUSY_TX){}
	HAL_UART_Transmit(&huart3, &msgChar, 1,1000);

}

/**
  * @brief Handles the user's command
  * @param user_input: User's input after pressing enter
  * @retval uint16_t: this will be passed to a queue to be processed by another task
  */
uint16_t handleCommand(const char * user_input){
	char help[] = "help";
	char mode[] = "mode";
	char scm[] = "scm";
	char fsm[] = "fsm";
	char atm[] = "atm";	//accelerated test mode

	char split_command[MAX_USER_INPUT];
	char full_command[MAX_USER_INPUT];
	uint16_t atm_period;
	bool error = false;

	for (int i = 0; i < MAX_USER_INPUT; ++i) {	//reset the char arrays
		split_command[i] = 0;
		full_command[i] = 0;
	}

	int i = 0;
	for (const char* p = user_input; *p; ++p) {	//assign user input to a char array
		full_command[i] = *p;
    	i++;
    }

	char* space = user_input;
	while (*space == ' ') {	//ignore leading whitespace
		space++;
	}
	user_input = space;

	i = 0;
	for (const char* p = user_input; *p; ++p) {	//takes the first parameter of user's input
		if ((strcmp(split_command, mode) == 0) || (strcmp(split_command, atm) == 0)){
			if (*p == ' ' || *p == '\r'){
				user_input = p+1;
				break;
			}
		}
		else {
			if (*p == ' ' || *p == '\r'){
				break;
			}
		}

		split_command[i] = *p;	//the rest of user's input
    	i++;
    }

	if (strcmp(split_command, help) == 0) {
		printStringBlocking("\r\nHelpful commands: \r\n");
		printStringBlocking("help:");
		printStringBlocking("\t\tdisplay a list of valid commands\r\n");

		printStringBlocking("mode fsm:");
		printStringBlocking("\tswitch to Failsafe mode\r\n");

		printStringBlocking("mode scm:");
		printStringBlocking("\tswitch to Static Cycle mode\r\n");

		printStringBlocking("atm x:");
		printStringBlocking("\t\tenter accelerated test mode with ");
		printStringBlocking("multiplication factor x (1-100)\r\n");
	}
	else if(strcmp(split_command, mode) == 0) {
		for (int i = 0; i < MAX_USER_INPUT; ++i) {	//reset input
			split_command[i] = 0;
		}

		i = 0;
		for (const char* p = user_input; *p; ++p) {
			if (*p == ' ' || *p == '\r'){
				user_input = p++;
				break;
			}

			split_command[i] =  *p;	//assign the second param of user input
			i++;
		}
		if(strcmp(fsm, split_command) == 0) {
			return FSM_MODE;
		}
		else if(strcmp(scm, split_command) == 0) {
			return SCM_MODE;
		}
		else {
			error = true;
		}
	}
	else if(strcmp(split_command, atm) == 0) {
		for (int i = 0; i < MAX_USER_INPUT; ++i) {
			split_command[i] = 0;
		}

		i = 0;
		for (const char* p = user_input; *p; ++p) {
			if (*p == ' ' || *p == '\r'){
				user_input = p++;
				break;
			}

			split_command[i] =  *p;
			i++;
		}

		atm_period = atoi(split_command);
		if (atm_period > 0 && atm_period <= 100) {
			return atm_period;
		}
		else {
			error = true;
		}
	}
	else if (split_command[0] = '\r'){	//if user presses enter with nothing else typed
		//do nothing
	}
	else {
		error = true;
	}


	if (error) {	//error check
		printStringBlocking("\r\nInvalid command");
	}

	return 0;
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
  uint16_t state = FSM_MODE;
  uint16_t period = DEFAULT_PERIOD;

  //Send a status message straight away
  statusMessage = state;
  if(osMessageQueuePut(Status_QueueHandle, &statusMessage, 1U, 0U)!= osOK)
  {
    Error_Handler();
  }
  /* Infinite loop */
  for(;;)
  {
	  int count = 0;
	//check for messages but do not block.
	status = osMessageQueueGet(CLI_QueueHandle, &cliMessage, NULL, 0U );
	if(status == osOK)
	{
		//This means a message has been received
		if (cliMessage <= ATM) {	//the message received is a period
			period = cliMessage;
		}

		if (cliMessage == FSM_MODE || cliMessage == SCM_MODE){//the message received is for the mode
			state = cliMessage;
		}

		statusMessage = period + state;	//adds these to be passed on to the queue

		if(osMessageQueuePut(Status_QueueHandle, &statusMessage, 1U, 0U)!= osOK)
	    {
		  Error_Handler();
		}

	}

	if (state == FSM_MODE){
		HAL_GPIO_WritePin(GPIOA, Red1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Red2_Pin, GPIO_PIN_RESET);
		osDelay(FSM_ON / period);
		HAL_GPIO_WritePin(GPIOA, Red1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, Red2_Pin, GPIO_PIN_SET);
		osDelay(FSM_OFF / period);
	}
	else if (state == SCM_MODE){	//reset is on set is off
		turnOffAllLights();
		HAL_GPIO_WritePin(GPIOA, Red1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Red2_Pin, GPIO_PIN_RESET);
		osDelay(ALL_RED_TIME / period);

		HAL_GPIO_WritePin(GPIOA, Red1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, Green1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, Blue1_Pin, GPIO_PIN_RESET);
		osDelay(PRI_WALK_TIME / period);


		HAL_TIM_Base_Start(&htim2);	//tim2 starts counting
		timer = __HAL_TIM_GET_COUNTER(&htim2);	//gets the counter value
		while(count < (PRI_WALK_WARN_TIME / period)) {
			if (__HAL_TIM_GET_COUNTER(&htim2) - timer >= (1000/ period)){
				count += 1000/period;
			}
			HAL_GPIO_TogglePin(GPIOA, Blue1_Pin);
			timer = __HAL_TIM_GET_COUNTER(&htim2);
			osDelay(WALK_BLINK_TIME/period);
		}


		HAL_GPIO_WritePin(GPIOA, Blue1_Pin, GPIO_PIN_SET);
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
		HAL_GPIO_WritePin(GPIOB, Blue2_Pin, GPIO_PIN_RESET);
		osDelay(SEC_WALK_TIME / period);


		count = 0;
		HAL_TIM_Base_Start(&htim2);	//tim2 starts counting
		timer = __HAL_TIM_GET_COUNTER(&htim2);	//gets the counter value
		while(count < (SEC_WALK_WARN_TIME / period)) {
			if (__HAL_TIM_GET_COUNTER(&htim2) - timer >= (1000 / period)){
				count += 1000/period;
			}
			HAL_GPIO_TogglePin(GPIOB, Blue2_Pin);
			timer = __HAL_TIM_GET_COUNTER(&htim2);
			osDelay(WALK_BLINK_TIME/period);
		}


		HAL_GPIO_WritePin(GPIOB, Blue2_Pin, GPIO_PIN_SET);
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
				cliMessage = handleCommand(cliBufferRX);

				//Assume that things are fine :)
				printStringBlocking("\r\n");
				printStringBlocking(PROMPT);

				//Send a message to the controller.

				if (cliMessage > 0){
					if(osMessageQueuePut(CLI_QueueHandle, &cliMessage, 1U, 0U)!= osOK)
					{
					  Error_Handler();
					}
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
	uint16_t period = 1;
	osStatus_t status;
	static char outstring[50];
	uint8_t mode[10];

	/* Infinite loop */
	for(;;)
	{
		status = osMessageQueueGet(Status_QueueHandle, &statusMessage, NULL, 0U );
		if(status == osOK)
		{
			if (statusMessage > SCM_MODE) {
				uint8_t state[] = "SCM Mode";
				strcpy(mode, state);
				period = statusMessage - SCM_MODE;
			}
			else if (statusMessage > FSM_MODE && statusMessage < SCM_MODE){
				uint8_t state[] = "FSM Mode";
				strcpy(mode, state);
				period = statusMessage - FSM_MODE;
			}
			else {	//startup
				uint8_t state[] = "FSM Mode";
				strcpy(mode, state);
				period = 1;
			}
		}

		printStringBlocking(SAVE_CURSOR);
		printStringBlocking("\x1b[1;1H");

		snprintf(outstring, 50,"Mode: %s \t Speed: x%i ", mode, period);
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
