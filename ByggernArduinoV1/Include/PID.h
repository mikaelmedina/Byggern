/*
 * PID.h
 *
 * Created: 28.10.2020 11:55:26
 *  Author: thomabsk
 */ 


#ifndef PID_H_
#define PID_H_


#include "sam.h"
#include "sam3x8e.h"


typedef struct PID_DATA{
	int16_t lastProcessValue;
	int32_t sumError;
	int16_t P_Factor;
	int16_t I_Factor;
	int16_t D_Factor;
	int16_t maxError;
	int32_t maxSumError;
} PID_DATA;

PID_DATA c_pid;

#define MAX_INT INT16_MAX
#define MAX_LONG INT32_MAX
#define MAX_I_TERM (MAX_LONG / 2)
#define PID_T 0.00006

void PID_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, PID_DATA *pid);
int16_t PID_Controller(int16_t setPoint, int16_t processValue, PID_DATA *pid);

#endif /* PID_H_ */