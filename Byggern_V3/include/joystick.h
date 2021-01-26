#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <util/delay.h>
#include <stdint.h>
#include "adc.h"
#include "math.h"
#include "can.h"

typedef struct JOY_POS {
	uint8_t x;
	uint8_t y;
}JOY_POS;


typedef struct JOY_POS_PERCENT {
	int8_t x;
	int8_t y;
}JOY_POS_PERCENT;

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NEUTRAL
} JOY_DIR;

JOY_POS joystick_getpos(void);
JOY_DIR joystick_getdir(void);
JOY_POS_PERCENT joystick_getpos_percent(void);
void joystick_calibrate(void);
uint8_t slider_getpos(ADC_CHANNEL slider_channel);
void joystick_send_pos(void);
void joystick_init(void);
int8_t joystick_get_right_button(void);
int8_t joystick_get_left_button(void);
int8_t joystick_get_joystick_button(void);
#endif /* JOYSTICK_H_ */