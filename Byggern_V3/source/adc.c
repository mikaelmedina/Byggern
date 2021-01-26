#include "adc.h"
#include <util/delay.h>
#include <avr/io.h>

#define F_CPU 4915200
#define BASE_ADDRESS 0x1400
#define ADC_BUSY 3 


void adc_init(void) {
	// PWM 16 bit
	DDRD |= (1 << PD5);
	TCCR1A = (0b11 << COM1A0)|(0b11 << COM1B0)|(0b00 << FOC1B)|(0b10 << WGM10);
	TCCR1B = (0 << ICNC1)|(0b0 << (ICES1))|(0b11 << WGM12)|(0b001 << CS10);
	ICR1 = 0x0A00; // Set frequency to 1920 Hz
	OCR1A = 0x0500; // Set duty cycle to 50 %

	// ADC busy pin
	DDRD |= (0 << ADC_BUSY); //Set pin as input
	PORTD |= (1 << ADC_BUSY); //Set pull up resistor
}

uint8_t adc_read(ADC_CHANNEL channel) {
	volatile uint8_t data = 0x00;
	volatile char *ext_adc = (char*) BASE_ADDRESS;
	ext_adc[0] = 0x80 | channel;
	//while(!(PIND & (1 << ADC_BUSY))); //Waiting for conversion
	_delay_ms(10);

	data = ext_adc[0];
	return data;
}
