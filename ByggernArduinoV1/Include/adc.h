/*
 * adc.h
 *
 * Created: 14.10.2020 16:39:30
 *  Author: thomabsk
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "sam.h"
#include "sam3x8e.h"
#include <stdint.h>
#include "game_controller.h"

typedef enum {
	IR_READING = 7
} ADC_CHANNEL;

void adc_init(void);
int get_score_flag(void);
void set_score_flag(int value);
uint32_t adc_read_channel(ADC_CHANNEL channel);

#endif /* ADC_H_ */