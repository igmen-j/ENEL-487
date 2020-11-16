/******************************************************************
 *
 * Filename: timer.h
 *
 * Description: A header file for the timer functions
 *
*******************************************************************/

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

int16_t timer_start(TIM_HandleTypeDef);
int16_t timer_stop(int16_t, TIM_HandleTypeDef);

#endif /* INC_TIMER_H_ */
