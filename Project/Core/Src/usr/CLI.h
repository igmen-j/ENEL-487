/*
 * CLI.h
 *
 * Author: Justin Igmen
 */

#ifndef SRC_USR_CLI_H_
#define SRC_USR_CLI_H_

#include "main.h"

#define MAX_USER_INPUT 				50
#define CLEAR_SCREEN 				"\x1b[2J"
#define PROMPT 						"cmd> "
#define SAVE_CURSOR 				"\x1b[s"
#define RESTORE_CURSOR 				"\x1b[u"

void welcomeMessage();
bool returnLineOrPartialLine (uint8_t*, uint8_t);

int cmd_counter;

#endif /* SRC_USR_CLI_H_ */
