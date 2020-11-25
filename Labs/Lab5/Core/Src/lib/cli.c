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
 *
 *********************************************************************/
void display_intro(UART_HandleTypeDef huart3) {
	uint8_t info1[] = "ENEL 487 Lab 5:\r\n";
	uint8_t info2[] = "RTOS Introduction\r\n\r\n";

	strcpy((char *) cliBufferTX, (char *) info1);
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, info2);
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
}

/*********************************************************************
 *
 * Function:    display_prompt()
 *
 * Description: Displays the prompt every line
 *
 *
 *********************************************************************/
void showPrompt(UART_HandleTypeDef huart3) {
	cmd_counter = 0;
	strcpy((char *) cliBufferTX, CMD_PROMPT);
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	reset_buffer();
}

bool buildCmd(UART_HandleTypeDef huart3) {
	uint8_t backspace[] = "\b";
	uint32_t test = 0;
	reset_buffer();
	while (cliBufferRX[0] != '\r'){
		if (HAL_UART_Receive(&huart3, cliBufferRX, 1, 1000) == HAL_OK) {
			//check if user enters backspace
			if (strcmp((char *) cliBufferRX, (char *) backspace) == 0){
				cliBufferTX[cmd_counter] = 0;	//clears the element
				--cmd_counter;
				test = cmd_counter;
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
		}
	}

	return false;
}

void doCommand(UART_HandleTypeDef huart3){
	uint8_t toggle[] = "toggle\r";
	uint8_t help[] = "help\r";
	uint8_t state[] = "state\r";
	uint8_t period[] = "period";
	uint8_t double_enter[] = "\r\n";
	uint32_t correct_input;
	int period_value_temp = 0;

	correct_input = sscanf (cliBufferTX, "%s %i", cmd, &period_value_temp);
	if (period_value_temp <= 0) {
		period_value_temp = 1;
	}

	if  (strcmp((char *) cliBufferTX, (char *) toggle) == 0) {
		//toggleLight();
	}
	else if (strcmp((char *) cliBufferTX, (char *) help) == 0){
		//Displays helpful commands to CLI
		displayHelp(huart3);
	}
	else if (strcmp((char *) cliBufferTX, (char *) state) == 0){
	  //checks if the LED is on or off and lets the user know
		getState();
	}
	else if (strcmp((char *) cmd, (char *) period) == 0 && correct_input == 2){
		period_value = period_value_temp;
		updatePeriod(huart3);
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

void getState() {
	GPIO_PinState on_or_off;
	on_or_off = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
	if (on_or_off == GPIO_PIN_RESET) {
		strcpy((char *) cliBufferTX, "\r\nLED is OFF");
	}
	else if (on_or_off == GPIO_PIN_SET) {
		strcpy((char *) cliBufferTX, "\r\nLED is ON");
	}
}

void displayHelp(UART_HandleTypeDef huart3) {
	strcpy((char *) cliBufferTX, "\r\nUseful Commands:\r\n");
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	//strcpy((char *) cliBufferTX, "toggle : ");
	//HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	//strcpy((char *) cliBufferTX, "turns LED on or off\r\n");
	//HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, "state : ");
	HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
	strcpy((char *) cliBufferTX, "displays the state of the LED");
}

void updatePeriod(UART_HandleTypeDef huart3) {
	strcpy((char *) cliBufferTX, "\r\nUpdating the period...");
	//HAL_UART_Transmit(&huart3, cliBufferTX, strlen((char *) cliBufferTX), 1000);
}
