#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include "sam.h"
#include "sam3x8e.h"
#include <stdint.h>
#include "encoder.h"
#include "adc.h"
#include "dacc.h"
#include "encoder.h"
#include "PID.h"
#include "timer.h"
#include "can_interrupt.h"

#define LOWER_BOUND			  0.0
#define UPPER_BOUND			200.0
#define UPPER_BOUND_SLIDER  255.0

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

typedef enum{
	JOYSTICK,
	SLIDER
}CONTROLLER_MODE;

JOY_POS_PERCENT current_joy_position;
JOY_DIR current_joy_dir;
int right_button;
uint8_t right_slider_pos;
int game_active;
CONTROLLER_MODE current_controller_mode;

void game_controller_init(void);
void game_controller_calibrate_motor(void);
void game_controller_update_pid(void);
void game_controller_update_pid_slider(void);
void game_controller_solenoid_pulse(void);

void game_controller_run_game(void);
void game_controller_servo_update(void);


#endif /* GAME_CONTROLLER_H_ */