/*
 * This source file contains user implemented delays (microseconds, milliseconds and seconds)
 * to be used in the project.
 */

#include "user_time.h"

void delay_ms(int milliseconds)
{
	int tick = HAL_GetTick();
	while ((HAL_GetTick() - tick) <= milliseconds); // do nothing
}

int checkTimeElapsed(int milliseconds)
{
	// This function checks for the time elapsed in milliseconds,
	// and returns true if specified time elapsed, otherwise false

	int check = 0;
	g_lastTick = HAL_GetTick();
	if (HAL_GetTick() - g_lastTick >= milliseconds) check = 1;

	return check;
}


