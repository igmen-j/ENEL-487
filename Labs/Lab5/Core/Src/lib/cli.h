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
#include <stdbool.h>

#define CMD_PROMPT "> "

uint8_t cliBufferTX[30];
uint8_t cliBufferRX[30];
uint8_t cmd[30];
uint32_t period_value;
int cmd_counter;

void reset_buffer();
void display_intro(UART_HandleTypeDef);
void showPrompt(UART_HandleTypeDef);
bool buildCmd(UART_HandleTypeDef);

void getState();
void displayHelp(UART_HandleTypeDef);
void updatePeriod(UART_HandleTypeDef);

#endif /* INC_CLI_H_ */
