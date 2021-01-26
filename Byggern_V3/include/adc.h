#ifndef ADC_H_
#define ADC_H_
#define F_CPU 4915200

#include <stdint.h>

typedef enum {
	JOYSTICK_X = 0,
	JOYSTICK_Y = 1,
	SLIDER_LEFT = 2,
	SLIDER_RIGHT = 3,
} ADC_CHANNEL;

void adc_init(void);
uint8_t adc_read(ADC_CHANNEL channel);

#endif /* ADC_H_ */