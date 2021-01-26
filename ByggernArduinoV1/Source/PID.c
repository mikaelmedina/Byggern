/*
 * PI.c
 *
 * Created: 28.10.2020 11:55:16
 *  Author: thomabsk
 */ 

#include "PID.h"
#include "stdint.h"

void PID_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, PID_DATA *pid)
// Set up PID controller parameters
{
	// Start values for PID controller
	pid->sumError = 0;
	pid->lastProcessValue = 0;
	// Tuning constants for PID loop
	pid->P_Factor = p_factor;
	pid->I_Factor = i_factor;
	pid->D_Factor = d_factor;
	// Limits to avoid overflow
	pid->maxError = MAX_INT / (pid->P_Factor + 1);
	pid->maxSumError = MAX_I_TERM / (pid->I_Factor + 1);
}


int16_t PID_Controller(int16_t setPoint, int16_t processValue, PID_DATA *pid){
	int16_t error, k_p, val;
	int32_t k_i;
	
	error = setPoint - processValue;
	pid->sumError += (int32_t) error;
	
	k_p = pid->P_Factor*error;
	k_i = PID_T * pid->I_Factor * pid->sumError;
	
	val = k_p + k_i;
	
	return val;
}