/*
 * This source file contains the functions that will be used in the project
 */
#include "project.h"

void startString()
{
	HAL_UART_Transmit(&huart2,g_startbuf,10, 1000);

} // end startString

void debounce()
{ // this function suppresses the bouncing of the switch.

	if ((button_now != button_prev) && (HAL_GetTick() - g_lastTick > 10))
	{
		g_lastTick = HAL_GetTick();
	} // end if

} // end checkBounce



void resetLEDs()
{ // this function turns all the LEDs off.
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);


} // end resetLEDs


void processFlags()
{ // this function processes the flags set-up in the handler.

	// record button
	if (rec_trig == 1)
	{
		rec_trig = 0;

		if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9) == GPIO_PIN_SET) button_now = 1;// if button pushed down and not released, set button_now to 1
		if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9) == GPIO_PIN_RESET) {button_now = 0,button_released = 1;} // if button released, set button_now to 0

		// debouncing
		debounce();
		if (button_now == 1) rec_flag = 1; // set flag that activates record state of the state machine
		else {rec_flag = 0;} // otherwise keep it at zero
		//		rec_trig = 0;
		button_prev = button_now;

	} // end processing of rec_trig flag

	// button one
	if (one_trig == 1)
	{

		// check the state of the pin
		if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == GPIO_PIN_SET) {button_now = 1, button_released = 0;}


		// debounce and check that the button has been released
		if ((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == GPIO_PIN_RESET) && (button_now == 1) && (HAL_GetTick() - g_lastTick > 10))
		{
			button_released = 1;
			g_lastTick = HAL_GetTick();
		}

		// evaluate and set playback condition
		if (button_released == 1 && (rec_flag != 1)) {
			one_flag = 1;
//			HAL_UART_Transmit(&huart2,g_playbackbuf[0],10, 1000);
			if(rec_flag != 1) HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,(uint32_t*)dacbuffer, 1024, DAC_ALIGN_12B_R); // start DAC
			button_released = 0;
			one_trig = 0;
		}
		// evaluate and set record condition
		if (button_released == 1 && (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9) == GPIO_PIN_SET)) {
			button_released = 0;
			rec_one_flag = 1;
//			HAL_UART_Transmit(&huart2,g_recordbuf[0],10, 1000);
			one_trig = 0;
		}

	}// end processing of one_trig flag

	// button two
	if (two_trig == 1)
	{
		// check the state of the pin
		if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) == GPIO_PIN_SET) {button_now = 1, button_released = 0;}

		// debounce and check that the button has been released
		if ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) == GPIO_PIN_RESET) && (button_now == 1) && (HAL_GetTick() - g_lastTick > 10))
		{
			button_released = 1;
			g_lastTick = HAL_GetTick();
		}// end if

		// evaluate and set playback condition
		if (button_released == 1 && (rec_flag != 1)) {
			two_flag = 1;
//			HAL_UART_Transmit(&huart2,g_playbackbuf[1],10, 1000);
			if(rec_flag != 1) HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,(uint32_t*)dacbuffer, 1024, DAC_ALIGN_12B_R);
			button_released = 0;
			two_trig = 0;
		} // end if
		// evaluate and set record condition
		if (button_released == 1 && (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9) == GPIO_PIN_SET)) {
			button_released = 0;
			rec_two_flag = 1;
//			HAL_UART_Transmit(&huart2,g_recordbuf[1],10, 1000);
			two_trig = 0;
		} // end if

	} // end processing of two_trig flag

	if (three_trig == 1)
	{
		// check the state of the pin
		if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_SET) {button_now = 1, button_released = 0;}


		// debounce and check that the button has been released
		if ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_RESET) && (button_now == 1) && (HAL_GetTick() - g_lastTick > 10))
		{
			button_released = 1;
			g_lastTick = HAL_GetTick();
		}

		// evaluate and set playback condition
		if (button_released == 1 && (rec_flag != 1)) {
			three_flag = 1;
//			HAL_UART_Transmit(&huart2,g_playbackbuf[2],10, 1000);
			if(rec_flag != 1) HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,(uint32_t*)dacbuffer, 1024, DAC_ALIGN_12B_R);
			button_released = 0;
			three_trig = 0;
		} // end if

		// evaluate and set record condition
		if (button_released == 1 && (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9) == GPIO_PIN_SET)) {
			button_released = 0;
			rec_three_flag = 1;
//			HAL_UART_Transmit(&huart2,g_recordbuf[2],10, 1000);
			three_trig = 0;
		}

	} // end processing of three_trig flag

	if (stop_trig == 1)
	{
		//check state of pin
		if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) == GPIO_PIN_SET) {button_now = 1, button_released = 0;}


		// debounce and check that the button has been released
		if ((HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) == GPIO_PIN_RESET) && (button_now == 1) && (HAL_GetTick() - g_lastTick > 10))
		{
			button_released = 1;
			g_lastTick = HAL_GetTick();
		}

		if (button_released == 1) {
			stop_flag = 1;
			button_released = 0;
			stop_trig = 0;
			resetStates();
			resetLEDs();
		}

	}
}  // end processFlags
