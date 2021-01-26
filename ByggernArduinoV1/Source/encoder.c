#include "encoder.h"

static int16_t encoder_value = 0x0000;

void encoder_init(void) {
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;

	//Setup pin 0 = OE, pin 1 = RST, pin 2 = SEL
	PIOD->PIO_PER |= PIO_PER_P0;
	PIOD->PIO_OER |= PIO_OER_P0;
	PIOD->PIO_SODR |= PIO_SODR_P0;


	PIOD->PIO_PER |= PIO_PER_P1;
	PIOD->PIO_OER |= PIO_OER_P1;

	PIOD->PIO_PER |= PIO_PER_P2;
	PIOD->PIO_OER |= PIO_OER_P2;
}

void encoder_reset(void) {
	PIOD->PIO_CODR |= PIO_CODR_P1;
	PIOD->PIO_SODR |= PIO_SODR_P1;
}

int encoder_read(void) {
	int state = 0;
	int reading_done = 0;
	int16_t enc = 0;

	while (!reading_done) {
		uint32_t I_flag = TC0->TC_CHANNEL[TIMER_CHANNEL_ENCODER].TC_SR;
		I_flag &= TC_SR_CPCS;

		if (I_flag) {
			switch (state) {
			case(0):
				//!OE low
				PIOD->PIO_CODR |= PIO_CODR_P0;
				//SEL low
				PIOD->PIO_CODR |= PIO_CODR_P2;
				TC0->TC_CHANNEL[TIMER_CHANNEL_ENCODER].TC_CCR |= TC_CCR_SWTRG;
				state++;
				break;
			case(1):
				enc += (int16_t)(0xFF00 & ((PIOC->PIO_PDSR) << 7));

				//SEL high
				PIOD->PIO_SODR |= PIO_SODR_P2;
				TC0->TC_CHANNEL[TIMER_CHANNEL_ENCODER].TC_CCR |= TC_CCR_SWTRG;
				state++;
				break;
			case(2):
				enc += (int16_t)(0x00FF & (PIOC->PIO_PDSR >> 1));

				encoder_reset();

				//!OE high
				PIOD->PIO_SODR |= PIO_SODR_P0;

				encoder_value = enc;
				enc = 0;
				TC0->TC_CHANNEL[TIMER_CHANNEL_ENCODER].TC_CCR |= TC_CCR_SWTRG;
				state = 0;
				reading_done = 1;
				break;
			}
		}
	}
}

int16_t encoder_get_encoder_value(void) {
	return encoder_value;
}
