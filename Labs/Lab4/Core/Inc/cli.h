/******************************************************************
 *
 * Filename: cli.h
 *
 * Description: A header file for the CLI functions
 *
*******************************************************************/

#ifndef INC_CLI_H_
#define INC_CLI_H_

#include "main.h"
#include "operations.h"

uint8_t cliBufferTX[20];
uint8_t cliBufferRX[20];
uint8_t cmd[20];
int cmd_counter;

void reset_buffer();
void display_intro(UART_HandleTypeDef);
void display_prompt(UART_HandleTypeDef);
void getCommand(UART_HandleTypeDef);
void displayCommand(UART_HandleTypeDef, TIM_HandleTypeDef);

#endif /* INC_CLI_H_ */
