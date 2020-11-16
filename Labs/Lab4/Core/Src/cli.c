/******************************************************************
 *
 * Filename:	cli.c
 *
 * Description: Functions for the CLI
 *
*******************************************************************/

#include "cli.h"

/*********************************************************************
 *
 * Function:    reset_buffer()
 *
 * Description: Clears the buffer for the command, transmit and receive variables
 *
 *********************************************************************/
void reset_buffer()
{
  for (int i = 0; i < 20; ++i) {
	  cliBufferTX[i] = 0;
	  cliBufferRX[i] = 0;
	  cmd[i] = 0;
  }
}

/*********************************************************************
 *
 * Function:    display_intro()
 *
 * Description: Displays the title when CLI is opened
 *
 * Params: 		huart2 - UART
 *
 *********************************************************************/
void display_intro(UART_HandleTypeDef huart2) {
	uint8_t info1[] = "ENEL 487 Lab 4:\r\n";
	uint8_t info2[] = "High-res Timing\r\n\r\n";

	strcpy((char *) cliBufferTX, (char *) info1);
	HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, info2);
	HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
}

/*********************************************************************
 *
 * Function:    display_prompt()
 *
 * Description: Displays the prompt every line
 *
 * Params: 		huart2 - UART
 *
 *********************************************************************/
void display_prompt(UART_HandleTypeDef huart2) {
	cmd_counter = 0;
	strcpy((char *) cliBufferTX, "> ");
	HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	reset_buffer();
}

/*********************************************************************
 *
 * Function:    getCommand()
 *
 * Description: Gets the received character input and builds the command
 *
 * Params: 		huart2 - UART
 *
 *********************************************************************/
void getCommand(UART_HandleTypeDef huart2) {
	uint8_t backspace[] = "\b";

	while (cliBufferRX[0] != '\r'){
		//only sends one character at a time -- tested sending multiple characters but I had trouble implementing it
		if (HAL_UART_Receive(&huart2, cliBufferRX, 1, 10000) == HAL_OK) {
		cliBufferTX[0] = cliBufferRX[0];

		//check if user enters backspace
		if (strcmp((char *) cliBufferTX, (char *) backspace) == 0){
			cmd[cmd_counter] = 0;	//clears the element
			--cmd_counter;

			//if counter is less than zero, remain at zero so that it stays at the current line
			if (cmd_counter < 0) {
				cmd_counter = 0;
			}
			else {
				HAL_UART_Transmit(&huart2, "\b \b", 3, 1000); //destructive backspace -- there may be a better way to do this
			}
		}
		else {
			//if not backspace, then enter it in the command line and increment it to next element and display
			cmd[cmd_counter] = cliBufferTX[0];
			++cmd_counter;
			HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
			}
		}
	}
}

/*********************************************************************
 *
 * Function:    displayCommand()
 *
 * Description: Displays the command entered by the user
 *
 * Params: 		huart2 - UART, htim4 - TImer
 *
 *********************************************************************/
void displayCommand(UART_HandleTypeDef huart2, TIM_HandleTypeDef htim4) {
	uint8_t toggle[] = "toggle\r";
	uint8_t help[] = "help\r";
	uint8_t state[] = "state\r";
	uint8_t process[] = "process\r";
	uint8_t double_enter[] = "\r\n\r\n";
	int averageTimes[9];

	GPIO_PinState on_or_off;

	//checks which command the user entered
	if  (strcmp((char *) cmd, (char *) toggle) == 0) {
		strcpy((char *) cliBufferTX, "\r\nToggle light");
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);	//turns on the light
	}
	else if (strcmp((char *) cmd, (char *) help) == 0){
		//Displays helpful commands to CLI
		strcpy((char *) cliBufferTX, "\r\nUseful Commands:\r\n");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		strcpy((char *) cliBufferTX, "toggle : ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		strcpy((char *) cliBufferTX, "turns LED on or off\r\n");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		strcpy((char *) cliBufferTX, "state : ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		strcpy((char *) cliBufferTX, "displays the state of the LED");
	}
	else if (strcmp((char *) cmd, (char *) state) == 0){
		//checks if the LED is on or off and lets the user know
		on_or_off = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
		if (on_or_off == GPIO_PIN_RESET) {
			strcpy((char *) cliBufferTX, "\r\nLED is OFF");
		}
		else if (on_or_off == GPIO_PIN_SET) {
			strcpy((char *) cliBufferTX, "\r\nLED is ON");
		}
	}
	else if (strcmp((char *) cmd, (char *) process) == 0){
		process_ops(htim4, averageTimes);

		strcpy((char *) cliBufferTX, "\r\nAverage time for each operation:\r\n");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\nAdd 32-bits: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[0]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\nAdd 64-bits: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[1]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\Multiply 32-bits: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[2]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\nMultiply 64-bits: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[3]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\Divide 32-bits: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[4]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\nDivide 64-bits: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[5]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\nCopy 8-byte struct: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[6]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\nCopy 128-byte struct: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[7]);
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);

		strcpy((char *) cliBufferTX, "\r\nCopy 1024-byte struct: ");
		HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
		sprintf(cliBufferTX, "%d", averageTimes[8]);
	}
	else if (cmd[0] == '\r') {
		//do nothing
	}
	else {
		//user entered an invalid command
		strcpy((char *) cliBufferTX, "\r\nInvalid command");
	}

	//display the output
	HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, double_enter);
	HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *) cliBufferTX), 1000);
}



