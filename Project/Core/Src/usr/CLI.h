/*
 * CLI.h
 *
 *  Created on: Dec 2, 2020
 *      Author: igmen
 */

#ifndef SRC_USR_CLI_H_
#define SRC_USR_CLI_H_

#include "main.h"

#define MAX_USER_INPUT 				100
#define CLEAR_SCREEN 				"\x1b[2J"
#define PROMPT 						"cmd> "
#define SAVE_CURSOR 				"\x1b[s"
#define RESTORE_CURSOR 				"\x1b[u"

char* GOTO_RC(uint32_t, uint32_t);
char* SET_SCROLL_ROW_TO_BOTTOM(uint32_t);
void welcomeMessage();
bool returnLineOrPartialLine (uint8_t*, uint8_t);

int cmd_counter;

#endif /* SRC_USR_CLI_H_ */
