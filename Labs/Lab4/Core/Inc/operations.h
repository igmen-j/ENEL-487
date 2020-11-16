/******************************************************************
 *
 * Filename: operations.h
 *
 * Description: A header file for the operations functions
 *
*******************************************************************/

#ifndef INC_OPERATIONS_H_
#define INC_OPERATIONS_H_

#include "main.h"
#include "timer.h"

struct byte8 {
	uint64_t a;
};

struct byte128 {
	uint64_t b[16];		//1024 bits / 64 bits = 16
};

struct byte1024 {
	uint64_t c[128];	//8192 bits / 64 bits = 128
};

void process_ops(TIM_HandleTypeDef htim4, int[9]);

#endif /* INC_OPERATIONS_H_ */
