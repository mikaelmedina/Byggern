/*
 * adc.c
 *
 * Created: 14.10.2020 16:39:18
 *  Author: thomabsk
 */ 

#include "adc.h"
#include "dacc.h"

static int score_flag = 0;
void ADC_Handler(void) {
	static int consecutive_readings = 0;
	uint32_t dummy = ADC->ADC_ISR;
	CAN_MESSAGE msg;
	
	consecutive_readings++;
	printf("ADC-INT: %d\n\r", adc_read_channel(7));
	if(game_active && consecutive_readings > 6) {
		game_active = 0;
		dacc_update_motor_speed(0);
		set_score_flag(1);
		msg.id = MSG_ID_POINT_SCORED;
		msg.data[0] = 0x1;
		msg.data_length = 1;
		can_send(&msg, 0);
		consecutive_readings = 0;
	}
	NVIC_ClearPendingIRQ(ADC_IRQn);
	WDT->WDT_CR |= (WDT_CR_KEY_PASSWD | WDT_CR_WDRSTT);
}

int get_score_flag(void){
	return score_flag;
}

void set_score_flag(int value){
	score_flag = value;
}

void adc_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;	
	
	//ADC->ADC_MR |= ADC_MR_TRGEN | ADC_MR_TRGSEL_ADC_TRIG2;
	ADC->ADC_MR |= ADC_MR_TRACKTIM(2) | ADC_MR_TRANSFER(0) | ADC_MR_FREERUN_ON | ADC_MR_PRESCAL(2);
	
	ADC->ADC_CR |= ADC_CR_START;

	ADC->ADC_CWR |= ADC_CWR_LOWTHRES(170) | ADC_CWR_HIGHTHRES(500);
	ADC->ADC_EMR |= ADC_EMR_CMPMODE_LOW | ADC_EMR_CMPSEL(7);
	ADC->ADC_IER |= ADC_IER_COMPE;

	ADC->ADC_CHER |= ADC_CHER_CH7;
}

uint32_t adc_read_channel(ADC_CHANNEL channel) {
	uint32_t data = ADC->ADC_CDR[channel] & 0x00000FFF;
	return data;
}
