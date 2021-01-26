/*
 * timer.h
 *
 * Created: 14.10.2020 08:50:02
 *  Author: thomabsk
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "sam.h"
#include "sam3x8e.h"


//Define what the timer channels are used for
#define TIMER_CHANNEL_PWM			0	//Used for generating PWM 
#define TIMER_CHANNEL_ENCODER		1	//Used for timed iterrupts when reading the encoder in the motorbox
#define TIMER_CHANNEL_PID			2	//Used for time stepping PID
#define TIMER_CHANNEL_SOLENOID		0   //Used for generating solenoid pulses

#define PWM_PERIOD				0.02	//Seconds
#define ENCODER_TIMER_VAL	 0.00002	//Seconds
#define PID_TIME_STEP		 0.00006	//Seconds
#define SOLENOID_PULSE_DURATION 0.24	//Seconds

#define MCK		  84000000 //Hz

#define PWM_MIN_DUTY	 4.5		
#define PWM_MAX_DUTY	10.5		

void timer_init(void);
void timer_PWM_duty_update(double duty);
void TC1_Handler(void);

#endif /* TIMER_H_ */