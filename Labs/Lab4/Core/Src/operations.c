/*
 * operations.c
 *
 *  Created on: Nov 14, 2020
 *      Author: igmen
 */

#include "operations.h"

int32_t random32bit() {
	return rand();
}

int64_t random64bit() {
	return rand();
}

void process_ops(TIM_HandleTypeDef htim4, int average[9]) {
	int16_t startTime, endTime;
	int32_t firstVal32, secondVal32, answer32;
	int64_t firstVal64, secondVal64, answer64;
	for (int i=0; i < 9; i++) {
		average[i] = 0;
	}

	struct byte8 structToCopy8, a8;
	struct byte128 structToCopy128, b128;
	struct byte1024 structToCopy1024, c1024;

	for (int i=0; i<100; i++) {
		firstVal32 = random32bit();
		secondVal32 = random32bit();
		startTime = timer_start(htim4);
		answer32 = firstVal32 + secondVal32;
		endTime = timer_stop(startTime, htim4);
		average[0] += endTime;

		firstVal64 = random64bit();
		secondVal64 = random64bit();
		startTime = timer_start(htim4);
		answer64 = firstVal64 + secondVal64;
		endTime = timer_stop(startTime, htim4);
		average[1] += endTime;

		firstVal32 = random32bit();
		secondVal32 = random32bit();
		startTime = timer_start(htim4);
		answer32 = firstVal32 * secondVal32;
		endTime = timer_stop(startTime, htim4);
		average[2] += endTime;

		firstVal64 = random64bit();
		secondVal64 = random64bit();
		startTime = timer_start(htim4);
		answer64 = firstVal64 * secondVal64;
		endTime = timer_stop(startTime, htim4);
		average[3] += endTime;

		firstVal32 = random32bit();
		secondVal32 = random32bit();
		startTime = timer_start(htim4);
		if (secondVal32 != 0){
		  answer32 = firstVal32 * secondVal32;
		}
		endTime = timer_stop(startTime, htim4);
		average[4] += endTime;

		firstVal64 = random64bit();
		secondVal64 = random64bit();
		startTime = timer_start(htim4);
		if (secondVal64 != 0){
		  answer64 += firstVal64 * secondVal64;
		}
		endTime = timer_stop(startTime, htim4);
		average[5] += endTime;

		startTime = timer_start(htim4);
		a8 = structToCopy8;
		endTime = timer_stop(startTime, htim4);
		average[6] += endTime;

		startTime = timer_start(htim4);
		b128 = structToCopy128;
		endTime = timer_stop(startTime, htim4);
		average[7] += endTime;

		startTime = timer_start(htim4);
		c1024 = structToCopy1024;
		endTime = timer_stop(startTime, htim4);
		average[8] += endTime;
	}

	for (int i=0; i < 9; i++) {
		average[i] /= 100;
	}
}
