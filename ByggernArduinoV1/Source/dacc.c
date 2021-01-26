/*
 * dacc.c
 *
 * Created: 21.10.2020 14:07:11
 *  Author: thomabsk
 */ 

#include "dacc.h"

// 25 clock periods to provide output
void dacc_init(void) {
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;
	
	// EN - Enables the motor 
	PIOD->PIO_PER |= PIO_PER_P9;
	PIOD->PIO_OER |= PIO_OER_P9;
	PIOD->PIO_SODR |= PIO_SODR_P9;
	
	// DIR - Sets the direction of the motor
	PIOD->PIO_PER |= PIO_PER_P10;
	PIOD->PIO_OER |= PIO_OER_P10;
	PIOD->PIO_SODR |= PIO_SODR_P10;
	
	// Setting P16 to be controlled by DACC
	PIOB->PIO_PDR |= PIO_PER_P16;
	
	PMC->PMC_PCER1 |= PMC_PCER1_PID38;
	DACC->DACC_MR = DACC_MR_TAG_EN;
	DACC->DACC_CHER = DACC_CHER_CH1;
}

static void dacc_update_motor(int16_t percent, int dir) {
	if (percent > 100) { percent = 100; }
	int val = (percent * ENCODER_MAX_VAL) / 100;

	DACC->DACC_CDR = ((0b11 & 0b1) << 12) | DACC_CDR_DATA(val);

	if (dir) {
		PIOD->PIO_SODR |= PIO_SODR_P10;
	}
	else {
		PIOD->PIO_CODR |= PIO_CODR_P10;
	}
}

static void dacc_update(int16_t val) {
	DACC->DACC_CDR = ((0b11 & 0b1) << 12) | DACC_CDR_DATA(val);
}

void dacc_update_motor_speed(int16_t input) {
	if (input > 0) {
		PIOD->PIO_CODR |= PIO_CODR_P10;
		dacc_update(input*20);
		//dacc_update_motor(0 - percent, 0);
	}
	else {
		PIOD->PIO_SODR |= PIO_SODR_P10;	
		dacc_update(input*-20);
		//dacc_update_motor(percent, 1);
	}
}

