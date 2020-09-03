/*
 * state_machine.h
 *
 *  Created on: 06 Mar 2020
 *      Author: 20687184
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_


//includes
#include "project.h"
#include "variables.h"
#include "sinewave.h"

 typedef enum
{
	 IdleState,
	 RecordState,
	 PlayState,
	 StopState


 }State; // Main states of the state machine

 State activeState; // this variable holds the current state of the state machine

typedef enum
{
	 RecordIdle,
	 RecordOne,
	 RecordTwo,
	 RecordThree

}Record; // Record states based on buttons one, two or three being pressed.

 Record activeRecordState; // variable to store the current record state i.e. one, two or three

 typedef enum
 {
	 PlayIdle,
	 PlayOne,
	 PlayTwo,
	 PlayThree
 }Play; //Play states based on buttons one, two or three being pressed

 Play activePlayState; // variable to store the current play state i.e. one, two or three
 Record activeRecordState;


// function prototypes
void stateMachine(void);
void states(void);
void resetStates();
void resetFlags();
void checkStop();
#endif /* INC_STATE_MACHINE_H_ */
