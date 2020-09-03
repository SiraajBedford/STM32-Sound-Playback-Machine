/*
 * This source file contains all the global variables to be used in the project
 */

#include "variables.h"

uint8_t recbuf[1024];//Buffer for the ADC, chose 1024
int32_t tempsample;
uint8_t outputbuf[1024];

UINT num;
int record_count=1723;//same for playback

//Things to consider-What is the value to subtract? ->average of all sampled values (Don't use hard coded average since it changes over time)
//Therefore calculate on-the-fly
int32_t average=128;//just an initial guess where average would be of signal WITH DC offset
int32_t accumulator;//in the callbacks ince we use it here
int32_t numavg;
float alpha=0.125;
float smoothed_sample;

FATFS fs;
FRESULT fres;
FIL sdfile;

uint8_t savestart=0;//for half callback
uint8_t savemid=0;//for full callback

uint8_t savestart_2=0;//for half callback
uint8_t savemid_2=0;//for full callback

uint8_t savestart_3=0;//for half callback
uint8_t savemid_3=0;//for full callback


uint8_t playstart=0;//for half callback
uint8_t playmid=0;//for full callback

uint8_t playstart_2=0;//for half callback
uint8_t playmid_2=0;//for full callback

uint8_t playstart_3=0;//for half callback
uint8_t playmid_3=0;//for full callback


// global variables
uint8_t g_startbuf[11] = {127,128,50,49,48,57,51,55,52,49};
uint8_t g_stopbuf[11] = {127,128,83,116,111,112,95,95,95,95};
uint8_t g_recordbuf[3][11] =
{
		{127,128,82,101,99,111,114,100,95,49},
		{127,128,82,101,99,111,114,100,95,50},
		{127,128,82,101,99,111,114,100,95,51}
};
uint8_t g_playbackbuf[3][11] =
{
		{127,128,80,108,97,121,95,95,95,49},
		{127,128,80,108,97,121,95,95,95,50},
		{127,128,80,108,97,121,95,95,95,51}
};

int g_lastTick = 0;

// buffers for sine wave generation
uint16_t dacbuffer[1024];

// STM32 CubeMX defined variables
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;
TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart2;



// volatiles
volatile int stop_flag = 0;// flag to check that stop button released
volatile int one_flag = 0; // flag to check that button one pressed
volatile int two_flag = 0; // flag to check that button two pressed
volatile int three_flag = 0; // flag to check that button three pressed
volatile int rec_flag = 0; // flag to set record state of state machine
volatile int rec_one_flag = 0; // flag to update record state to record to slot one
volatile int rec_two_flag = 0; // flag to update record state to record to slot two
volatile int rec_three_flag = 0; // flag to update record state to record to slot three
volatile int play_one_flag = 0;
volatile int play_two_flag = 0;
volatile int play_three_flag = 0;


// interrupt flags, each one corresponds to either of the record, stop, one, two, or three buttons being pressed.
volatile int rec_trig = 0;
volatile int stop_trig = 0;
volatile int one_trig = 0;
volatile int two_trig = 0;
volatile int three_trig = 0;


//some test variables for debouncing
volatile int button_now = 0;
volatile int button_prev = 0;
volatile int button_released = 0;
volatile int one_debounce = 0;
