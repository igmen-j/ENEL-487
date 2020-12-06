/*
 * CLI.c
 *
 *  Created on: Dec 2, 2020
 *      Author: igmen
 */

#include "CLI.h"

char* GOTO_RC(uint32_t row, uint32_t col) {
	col = 5;
	char escape[] = "\x1b[";
	char* return_val = escape;
	return_val += sizeof(char) * 2;
	*return_val = (char *)row;
	return_val += sizeof(char);
	*return_val = ';';
	return_val += sizeof(char);
	*return_val = (char *)col;
	return_val += sizeof(char);
	*return_val = 'H';
	return_val += sizeof(char) * 5;

	//strcat((char *)escape, row + '\0');
	//strcat((char *)escape, col + '\0');

	//sprintf(escape, "\x1b[%i;%iH", (char*)row, (char*)col);
	return return_val;
}
char* SET_SCROLL_ROW_TO_BOTTOM(uint32_t row) {
	char * escape;
	sprintf(escape, "\x1b[%i;r", row);
	return escape[0];
}

void welcomeMessage() {
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
			return true;
		}
	}

	return false;
}
