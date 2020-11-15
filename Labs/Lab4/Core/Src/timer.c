#include "timer.h"

int16_t timer_start(TIM_HandleTypeDef htim4) {
	return __HAL_TIM_GET_COUNTER(&htim4);
}

int16_t timer_stop(int16_t startTime, TIM_HandleTypeDef htim4) {
	return __HAL_TIM_GET_COUNTER(&htim4) - startTime;
}
