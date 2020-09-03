/*
 *
 * This header file contains user implemented delays (microseconds, milliseconds and seconds)
 * to be used in the project.
 *
 */

#ifndef INC_USER_TIME_H_
#define INC_USER_TIME_H_

// includes
#include "main.h"
#include "stdbool.h"
#include "variables.h"
// function prototypes
void delay_ms(int milliseconds);
int checkTimeElapsed(int milliseconds);


#endif /* INC_USER_TIME_H_ */
