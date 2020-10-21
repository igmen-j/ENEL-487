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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>

#define CMD_PROMPT "=> "	//change this if you want a different prompt

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
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

uint8_t cliBufferTX[40];
uint8_t cliBufferRX[40];
uint8_t infoTXBuffer[40];
int cmd_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void reset_buffer(void);
void showPrompt(void);
bool buildCmd();
void doCommand();
void toggleLight();
void displayHelp();
void getState();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * Purpose: resets the buffer for RX and TX
 */
void reset_buffer()
{
  for (int i = 0; i < 20; ++i) {
	  cliBufferTX[i] = 0;
	  cliBufferRX[i] = 0;
  }
}

/*
 * Purpose: resets the buffer for the counter
 */
void reset_info_buffer()
{
  for (int i = 0; i < 20; ++i) {
	  infoTXBuffer[i] = 0;
  }
}

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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  uint8_t info1[] = "ENEL 487 Lab 3: ";
  uint8_t info2[] = "CLI Part 2 on the Target\r\n\r\n";
  uint8_t timeInfo[] = "Seconds since running: ";

  strcpy((char *) infoTXBuffer, (char *) info1);
  HAL_UART_Transmit(&huart3, infoTXBuffer, strlen((char *) infoTXBuffer), 1000);
  strcpy((char *) infoTXBuffer, info2);
  HAL_UART_Transmit(&huart3, infoTXBuffer, strlen((char *) infoTXBuffer), 1000);

  uint8_t counter = 0;
  strcpy((char *) infoTXBuffer, (char *) timeInfo);
  HAL_UART_Transmit(&huart3, infoTXBuffer, strlen((char *) infoTXBuffer), 1000);

  HAL_UART_Transmit(&huart3, "\x1b[8;r", 5, 1000);	//creates a scrollable cmd line
  HAL_UART_Transmit(&huart3, "\x1b[8;0H", 6, 1000);	//moves the cursor to row eight
  HAL_UART_Transmit(&huart3, "\x1b[1K", 4, 1000);	//clears the line

  showPrompt();

  //code above is to display info for the user

  HAL_UART_Receive_IT(&huart3, &cliBufferRX, 1);	//start the non-blocking receive interrupt

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  reset_info_buffer();
  reset_buffer();

  uint8_t test[20];
  while (1)
  {
	  HAL_UART_Transmit(&huart3, "\x1b[s", 3, 1000);	//saves the current position of the cursor
	  HAL_UART_Transmit(&huart3, "\x1b[3;24f", 7, 1000); //moves the cursor to the coutner position
	  reset_info_buffer();
	  sprintf(test, "%d", counter);
	  HAL_UART_Transmit(&huart3, test, strlen((char *) test), 1000);
	  HAL_UART_Transmit(&huart3, "\x1b[u", 3, 1000); //returns the cursor back to the cmd line
	  HAL_Delay(1000);
	  counter += 1;
	  //showPrompt();
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
 * Purpose: this function will be called when there is a receive interrupt
 * 		Handles all of the command from user
 */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * husart) {
	bool isEnterPressed = false;
	isEnterPressed = buildCmd();	//checks if enter key is pressed
	if (isEnterPressed) {
		doCommand();
		showPrompt();
	}

	while (huart3.gState == HAL_UART_STATE_BUSY_RX){}
	HAL_UART_Receive_IT (&huart3, &cliBufferRX, 1); //restarts the receive interrupt
}

/*
 * Purpose: Tracks the keys user has pressed and display them to the CLI
 * Output: true if enter key is pressed and false otherwise
 */
bool buildCmd() {
	uint8_t backspace[] = "\b";

	//check if user enters backspace
	if (strcmp((char *) cliBufferRX, (char *) backspace) == 0){
		cliBufferTX[cmd_counter] = 0;	//clears the element
		--cmd_counter;

		//if counter is less than zero, remain at zero so that it stays at the current line
		if (cmd_counter < 0) {
			cmd_counter = 0;
		}
		else {
			HAL_UART_Transmit(&huart3, "\b \b", 3, 1000); //destructive backspace -- there may be a better way to do this
		}
	}
	else {
	//if not backspace, then enter it in the command line and increment it to next element and display
		cliBufferTX[cmd_counter] = cliBufferRX[0];
		++cmd_counter;
		HAL_UART_Transmit(&huart3, cliBufferRX, 1, 1000);

		if (cliBufferRX[0] == '\r') {
			return true;
		}
	}

	return false;
}

/*
 * Purpose: Displays the cmd line prompt
 * See #define above if you would like to change it
 */
void showPrompt() {
	HAL_UART_Transmit(&huart3, CMD_PROMPT, strlen((char *) 3), 1000);
}

/*
 * Purpose: Handles the command the user has entered
 */
void doCommand(){
	uint8_t toggle[] = "toggle\r";
	uint8_t help[] = "help\r";
	uint8_t state[] = "state\r";
	uint8_t double_enter[] = "\r\n";

	if  (strcmp((char *) cliBufferTX, (char *) toggle) == 0) {
		toggleLight();
	}
	else if (strcmp((char *) cliBufferTX, (char *) help) == 0){
		//Displays helpful commands to CLI
		displayHelp();
	}
	else if (strcmp((char *) cliBufferTX, (char *) state) == 0){
	  //checks if the LED is on or off and lets the user know
		getState();
	}
	else if (strcmp((char *) cliBufferTX, "\r") == 0) {
		strcpy((char *) cliBufferTX, "");
	}
	else {
		//user entered an invalid command
		strcpy((char *) cliBufferTX, "\r\nInvalid command");
	}

	  //display the output
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, double_enter);
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);

	reset_buffer();
	cmd_counter = 0;
}

/*
 * Purpose: Toggles the LED
 * Will run if the user enters "toggle"
 */
void toggleLight() {
	strcpy((char *) cliBufferTX, "\r\nToggle light\r\n");
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);	//turns on the light
}

/*
 * Purpose: Displays available commands to the user
 * Will run if the user enters "help"
 */
void displayHelp() {
	strcpy((char *) cliBufferTX, "\r\nUseful Commands:\r\n");
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, "toggle : ");
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, "turns LED on or off\r\n");
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, "state : ");
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, "displays the state of the LED\r\n");
}

/*
 * Purpose: Lets the user know if the LED is on or off
 * Will run if the user enters "state"
 */
void getState() {
	GPIO_PinState on_or_off;
	on_or_off = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
	if (on_or_off == GPIO_PIN_RESET) {
		strcpy((char *) cliBufferTX, "\r\nLED is OFF\r\n");
	}
	else if (on_or_off == GPIO_PIN_SET) {
		strcpy((char *) cliBufferTX, "\r\nLED is ON\r\n");
	}
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
