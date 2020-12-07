/*
 * CLI.c
 *
 * Author: Justin Igmen
 */

#include "CLI.h"

void welcomeMessage() {
	printStringBlocking("\x1b[0;0H");
	printStringBlocking("ENEL 487 Project\r\n");
	printStringBlocking("Justin Igmen");
}

bool returnLineOrPartialLine (uint8_t* cliBufferRX, uint8_t cliRXChar) {
	uint8_t backspace = '\b';

	//check if user enters backspace
	if (strcmp((char *) cliRXChar, (char *) backspace) == 0){
		cliBufferRX[cmd_counter] = 0;	//clears the element
		--cmd_counter;

		//if counter is less than zero, remain at zero so that it stays at the current line
		if (cmd_counter < 0) {
			cmd_counter = 0;
		}
		else {
			sendByte('\b');//destructive backspace
			sendByte(' ');
			sendByte('\b');
		}
	}
	else {
	//if not backspace, then enter it in the command line and increment it to next element and display
		cliBufferRX[cmd_counter] = cliRXChar;
		++cmd_counter;
		sendByte(cliRXChar);

		if (cliRXChar == '\r') {
			cmd_counter = 0;
			return true;
		}
	}

	return false;
}
