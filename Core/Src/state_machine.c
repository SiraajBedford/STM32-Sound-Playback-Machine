/*
 * state_machine.c
 *
 * This source file contains all the code relevant to the implementation of a state machine for the project.
 */

#include "state_machine.h"
#include "variables.h"

void states(void){
	if (rec_flag == 1) activeState = RecordState;
	if (stop_flag == 1) activeState = StopState;
	if (one_flag || two_flag || three_flag) activeState = PlayState;
} // end states


void stateMachine(void){

	switch (activeState){

	case RecordState: {
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);

		switch (activeRecordState){


		case RecordIdle: {

			if(rec_one_flag)
			{
				activeRecordState = RecordOne;
				HAL_UART_Transmit(&huart2,g_recordbuf[0],10, 1000);
			}
			if(rec_two_flag) {
				activeRecordState = RecordTwo;
				HAL_UART_Transmit(&huart2,g_recordbuf[1],10, 1000);
			}
			if(rec_three_flag){
				activeRecordState = RecordThree;
				HAL_UART_Transmit(&huart2,g_recordbuf[2],10, 1000);
			}
			break;
		}


		case RecordOne: {
			// if record button pressed and held, and button one pressed and released, do the following:

			//mount file system (SD card)
			fres=f_mount(&fs, "", 1);

			//In full project implementation we have to use buttons for playback
			fres=f_open(&sdfile, "record1.bin", FA_CREATE_ALWAYS | FA_WRITE);//Combination of flag ovewrites file if present already in memory

			if(savestart == 1){
				fres=f_write(&sdfile, outputbuf, 512, &num);
				savestart=0;
				record_count--;
			}

			if(savemid == 1){
				fres=f_write(&sdfile, outputbuf+512, 512, &num);
				savemid=0;
				record_count--;

			}

			if(record_count == 0){
				rec_one_flag = 0; // reset flag=0;
				f_close(&sdfile);
			}


			HAL_TIM_Base_Start_IT(&htim3); // trigger interrupt to toggle LED <1>
			if (activeState == StopState) break;
			break;
		} // end case RecordOne


		case RecordTwo: {
			// if record button pressed and held, and button two pressed and released, do the following:

			//mount file system (SD card)
			fres=f_mount(&fs, "", 1);

			//In full project implementation we have to use buttons for playback
			fres=f_open(&sdfile, "record2.bin", FA_CREATE_ALWAYS | FA_WRITE);//Combination of flag overwrites file if present already in memory

			if(savestart_2 == 1){
				fres=f_write(&sdfile, outputbuf, 512, &num);
				savestart_2=0;
				record_count--;
			}

			if(savemid_2 == 1){
				fres=f_write(&sdfile, outputbuf+512, 512, &num);
				savemid_2=0;
				record_count--;

			}

			if(record_count == 0){
				rec_two_flag = 0; // reset flag=0;
				f_close(&sdfile);
			}

			HAL_TIM_Base_Start_IT(&htim3);
			if(activeState == StopState) break;
			break;
		} // end case RecordTwo


		case RecordThree: {

			// if record button pressed and held, and button two pressed and released, do the following:

			//mount file system (SD card)
			fres=f_mount(&fs, "", 1);

			//In full project implementation we have to use buttons for playback
			fres=f_open(&sdfile, "record3.bin", FA_CREATE_ALWAYS | FA_WRITE);//Combination of flag overwrites file if present already in memory

			if(savestart_3 == 1){
				fres=f_write(&sdfile, outputbuf, 512, &num);
				savestart_3=0;
				record_count--;
			}

			if(savemid_3 == 1){
				fres=f_write(&sdfile, outputbuf+512, 512, &num);
				savemid_3=0;
				record_count--;

			}

			if(record_count == 0){
				rec_three_flag = 0; // reset flag=0;
				f_close(&sdfile);
			}


			HAL_TIM_Base_Start_IT(&htim3);
			if(activeState == StopState) break;
			break;
		} // end case RecordThree

		} // end nested switch

		break;
	} // end case RecordState



	case StopState: {
		stop_flag = 0;
		resetLEDs();
		HAL_TIM_Base_Stop_IT(&htim3);
		HAL_UART_Transmit(&huart2,g_stopbuf,10, 1000);
		HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_1); // stop DAC peripheral
		resetFlags();
		activeState = IdleState;
		break;
	} // end case StopState




	case PlayState: {
		switch (activePlayState){

		case PlayIdle: {
			if (one_flag){
				activePlayState = PlayOne;
				HAL_UART_Transmit(&huart2,g_playbackbuf[0],10, 1000);
			}
			if (two_flag) {
				activePlayState = PlayTwo;
				HAL_UART_Transmit(&huart2,g_playbackbuf[1],10, 1000);
			}
			if(three_flag) {
				activePlayState = PlayThree;
				HAL_UART_Transmit(&huart2,g_playbackbuf[2],10, 1000);
			}
			break;
		} // end case PlayIdle


		case PlayOne: {

			// if record button pressed and held, and button one pressed and released, do the following:

			//mount file system (SD card)
			fres=f_mount(&fs, "", 1);

			//In full project implementation we have to use buttons for playback
			fres=f_open(&sdfile, "record1.bin", FA_OPEN_ALWAYS | FA_READ);//Combination of flag ovewrites file if present already in memory

			if(playstart == 1){
				fres=f_read(&sdfile, dacbuffer, 512, &num);
				playstart=0;
				record_count--;
			}

			if(playmid == 1){
				fres=f_read(&sdfile, dacbuffer+512, 512, &num);
				playmid=0;
				record_count--;

			}

			if(record_count == 0){
				one_flag = 0; // reset flag=0;
				f_close(&sdfile);
			}


			HAL_TIM_Base_Start_IT(&htim3);
			if(activeState == StopState) break; // check for stop condition
			break;
		} // end case PlayOne



		case PlayTwo: {

			// if record button pressed and held, and button one pressed and released, do the following:

			//mount file system (SD card)
			fres=f_mount(&fs, "", 1);

			//In full project implementation we have to use buttons for playback
			fres=f_open(&sdfile, "record2.bin", FA_OPEN_ALWAYS | FA_READ);//Combination of flag ovewrites file if present already in memory

			if(playstart_2 == 1){
				fres=f_read(&sdfile, dacbuffer, 512, &num);
				playstart_2=0;
				record_count--;
			}

			if(playmid_2 == 1){
				fres=f_read(&sdfile, dacbuffer+512, 512, &num);
				playmid_2=0;
				record_count--;

			}

			if(record_count == 0){
				two_flag = 0;// reset flag=0;
				f_close(&sdfile);
			}

			HAL_TIM_Base_Start_IT(&htim3);
			if(activeState == StopState) break;

			break;
		} // end case PlayTwo




		case PlayThree: {

			// if record button pressed and held, and button one pressed and released, do the following:

			//mount file system (SD card)
			fres=f_mount(&fs, "", 1);

			//In full project implementation we have to use buttons for playback
			fres=f_open(&sdfile, "record3.bin", FA_OPEN_ALWAYS | FA_READ);//Combination of flag ovewrites file if present already in memory

			if(playstart_3 == 1){
				fres=f_read(&sdfile, dacbuffer, 512, &num);
				playstart_3=0;
				record_count--;
			}

			if(playmid_3 == 1){
				fres=f_read(&sdfile, dacbuffer+512, 512, &num);
				playmid_3=0;
				record_count--;

			}

			if(record_count == 0){
				three_flag = 0;// reset flag=0;
				f_close(&sdfile);
			}


			HAL_TIM_Base_Start_IT(&htim3);
			if(activeState == StopState) break;
			break;
		} // end case PlayThree
		} // end nested switch
		break;
	} // end case PlayState




	case IdleState: {
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
		resetStates();
		break;
	} // end case IdleState
	} // end switch

} // end stateMachine



void resetFlags()
{ // this function resets all the flags used to determine states.
	rec_flag = 0;
	stop_flag = 0;
	one_flag = 0;
	two_flag = 0;
	three_flag = 0;
	savestart=0;//for half callback
	savemid=0;//for full callback
	savestart_2=0;//for half callback
	savemid_2=0;//for full callback
	savestart_3=0;//for half callback
	savemid_3=0;//for full callback

	playstart=0;//for half callback
	playmid=0;//for full callback
	playstart_2=0;//for half callback
	playmid_2=0;//for full callback
	playstart_3=0;//for half callback
	playmid_3=0;//for full callback

} // end resetFlags



void resetStates()
{
	activeState = IdleState;
	activeRecordState = RecordIdle;
	activePlayState = PlayIdle;
}



void checkStop()
{ // this function checks that the stop button has been pressed and released, and puts the state machine into the Stop state
	if (stop_flag == 1)
	{
		activeState = StopState;
		activeRecordState = RecordIdle;
		activePlayState = PlayIdle;
	} // end if
} // end checkStop
