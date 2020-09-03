/*
 * This header file declarations for all the global variables to be used in the project
 *
 */

#ifndef INC_VARIABLES_H_
#define INC_VARIABLES_H_

// includes
#include "main.h"
#include "stdbool.h"
#include "variables.h"
#include "project.h"
#include "state_machine.h"
#include "sinewave.h"
#include "sd.h"
#include "main.h"
#include "fatfs.h"

// variables

extern uint8_t g_startbuf[11];
extern uint8_t g_playbackbuf[3][11];
extern uint8_t g_recordbuf[3][11];
extern uint8_t g_stopbuf[11];
extern int g_lastTick;


// STMCube generated variables
extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_dac1;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;


// volatiles
extern volatile int rec_flag;
extern volatile int stop_flag;
extern volatile int one_flag;
extern volatile int two_flag;
extern volatile int three_flag;
extern volatile int rec_one_flag;
extern volatile int rec_two_flag;
extern volatile int rec_three_flag;
extern volatile int play_one_flag;
extern volatile int play_two_flag;
extern volatile int play_three_flag;

//interrupt flags
extern volatile int rec_trig;
extern volatile int stop_trig;
extern volatile int one_trig;
extern volatile int two_trig;
extern volatile int three_trig;

// buffers for sine wave generation
extern uint16_t dacbuffer[1024];

// variables for debouncing
extern volatile int button_now;
extern volatile int button_prev;
//extra variables
extern volatile int button_released;

extern uint8_t recbuf[1024];//Buffer for the ADC, chose 1024
extern int32_t tempsample;
extern uint8_t outputbuf[1024];

//Things to consider-What is the value to subtract? ->average of all sampled values (Don't use hard coded average since it changes over time)
//Therefore calculate on-the-fly
extern int32_t average;//just an initial guess where average would be of signal WITH DC offset
extern int32_t accumulator;//in the callbacks ince we use it here
extern int32_t numavg;
extern float alpha;
extern float smoothed_sample;

extern UINT num;
extern int record_count;

extern FATFS fs;
extern FRESULT fres;
extern FIL sdfile;

extern uint8_t savestart;//for half callback
extern uint8_t savemid;//for full callback

extern uint8_t savestart_2;//for half callback
extern uint8_t savemid_2;//for full callback

extern uint8_t savestart_3;//for half callback
extern uint8_t savemid_3;//for full callback


extern uint8_t playstart;//for half callback
extern uint8_t playmid;//for full callback

extern uint8_t playstart_2;//for half callback
extern uint8_t playmid_2;//for full callback

extern uint8_t playstart_3;//for half callback
extern uint8_t playmid_3;//for full callback


#endif /* INC_VARIABLES_H_ */
