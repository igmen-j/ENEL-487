/******************************************************************
 *
 * Filename:	timer.c
 *
 * Description: Functions for the timer
 *
*******************************************************************/

#include "timer.h"

/*********************************************************************
 *
 * Function:    timer_start()
 *
 * Description: Starts the timer
 *
 * Params: 		htim4 - TImer
 *
 *********************************************************************/
int16_t timer_start(TIM_HandleTypeDef htim4) {
	return __HAL_TIM_GET_COUNTER(&htim4);
}

/*********************************************************************
 *
 * Function:    timer_stop()
 *
 * Description: Stops the timer
 *
 * Params: 		htim4 - TImer
 *
 *********************************************************************/
int16_t timer_stop(int16_t startTime, TIM_HandleTypeDef htim4) {
	return __HAL_TIM_GET_COUNTER(&htim4) - startTime;
}
