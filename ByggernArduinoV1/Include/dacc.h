/*
 * dacc.h
 *
 * Created: 21.10.2020 14:06:54
 *  Author: thomabsk
 */ 


#ifndef DACC_H_
#define DACC_H_

#include "sam.h"
#include "sam3x8e.h"
#include <stdint.h>

#define ENCODER_MAX_VAL 0xFFF

void dacc_init(void);
//dir = 0 -> reverse
//void dacc_update_motor_speed(uint8_t percent, int dir);
void dacc_update_motor_speed(int16_t input);

#endif /* DACC_H_ */