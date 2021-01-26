/*
 * game_controller.c
 *
 * Created: 21.10.2020 17:22:39
 *  Author: thomabskdwdd
 */ 

#include "game_controller.h"

static int16_t encoder_max, encoder_min;
static double linear_a, linear_b, linear_a_slider, linear_b_slider;

void game_controller_init(void) {
	//Enable P13 as solenoid output
	PIOC->PIO_PER |= PIO_PER_P13;
	PIOC->PIO_OER |= PIO_OER_P13;
	PMC->PMC_PCER0 |= PMC_PCER0_PID13;
	PIOC->PIO_CODR |= PIO_CODR_P13;

	//Enable P19 - yellow LED and P20 - green LED
	PIOA->PIO_PER |= PIO_PER_P19;
	PIOA->PIO_OER |= PIO_OER_P19;
	PIOA->PIO_PER |= PIO_PER_P20;
	PIOA->PIO_OER |= PIO_OER_P20;
	PMC->PMC_PCER0 |= PMC_PCER0_PID11;

	// Yellow led indicating game is initializing
	PIOA->PIO_SODR |= PIO_SODR_P19;

	timer_init();
	adc_init();
	dacc_init();
	PID_Init(1, 3, 0, &c_pid);
	encoder_init();
	game_controller_calibrate_motor();

	// Green LED indication calibration is done and game is ready
	PIOA->PIO_SODR |= PIO_SODR_P20;
}


void game_controller_calibrate_motor(void){
	// Drive motor to the right and reset, to get minimum value of encoder
	dacc_update_motor_speed(-60);
	for(int i = 0; i<100000000;i++);
	dacc_update_motor_speed(0);
	encoder_reset(); 
	encoder_read();
	encoder_min = encoder_get_encoder_value();

	// Drive motor to the left and reset, to get max value of encoder
	dacc_update_motor_speed(60);
	for (int i = 0; i < 100000000; i++);
	dacc_update_motor_speed(0);
	encoder_reset();
	encoder_read();
	encoder_max = encoder_get_encoder_value();
		
	linear_a = ((UPPER_BOUND - LOWER_BOUND) / ((double)encoder_max - (double)encoder_min));
	linear_b = LOWER_BOUND - linear_a*(double)encoder_min;

	linear_a_slider = ((UPPER_BOUND_SLIDER - LOWER_BOUND) / ((double)encoder_max - (double)encoder_min));
	linear_b_slider = LOWER_BOUND - linear_a*(double)encoder_min;
}


void game_controller_update_pid(void){
	uint32_t I_flag = TC0->TC_CHANNEL[TIMER_CHANNEL_PID].TC_SR;
	I_flag &= TC_SR_CPCS;
	if(I_flag) {
		int enc_val = (int)(linear_a*(encoder_get_encoder_value())+ linear_b);

		int16_t system_input = PID_Controller((current_joy_position.y - 100)*-1, enc_val, &c_pid);
		dacc_update_motor_speed(system_input);
	}
	TC0->TC_CHANNEL[TIMER_CHANNEL_PID].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;
}


void game_controller_update_pid_slider(void){
	uint32_t I_flag = TC0->TC_CHANNEL[TIMER_CHANNEL_PID].TC_SR;
	I_flag &= TC_SR_CPCS;
	if(I_flag) {
		int enc_val = (int)(linear_a_slider*(encoder_get_encoder_value())+ linear_b_slider);
		
		int16_t system_input = PID_Controller((((int16_t)(right_slider_pos))-255)*-1, enc_val, &c_pid);
		dacc_update_motor_speed(system_input);
	}
	TC0->TC_CHANNEL[TIMER_CHANNEL_PID].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;
}


void game_controller_run_game(void) {
	CAN_MESSAGE msg_game_stopped;

	while (1) {
		while (!game_active) {
			WDT->WDT_CR |= (WDT_CR_KEY_PASSWD | WDT_CR_WDRSTT);
		}
		encoder_read();
		if(current_controller_mode == JOYSTICK){
			game_controller_update_pid();
		}
		else if(current_controller_mode == SLIDER){
			game_controller_update_pid_slider();
		}
		game_controller_solenoid_pulse();
		game_controller_servo_update();
		
		if (get_score_flag()) {
			set_score_flag(0);
		}
		WDT->WDT_CR |= (WDT_CR_KEY_PASSWD | WDT_CR_WDRSTT);
	}
}


void game_controller_solenoid_pulse(void) {
	static int right_button_flag = 1;
	uint32_t I_flag = TC1->TC_CHANNEL[TIMER_CHANNEL_SOLENOID].TC_SR;
	I_flag &= TC_SR_CPCS;
	if(I_flag) {
		PIOC->PIO_CODR |= PIO_CODR_P13;
		right_button_flag = 1;
	}
	
	printf("Timer: %d\n\r", TC1->TC_CHANNEL[TIMER_CHANNEL_SOLENOID].TC_CV);
	
	if(right_button && right_button_flag){
		TC1->TC_CHANNEL[TIMER_CHANNEL_SOLENOID].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;
		PIOC->PIO_SODR |= PIO_SODR_P13;
		right_button_flag = 0;
	}

}

void game_controller_servo_update(void) {
	int duty = 751800 + (100 + current_joy_position.x)*252;

	if(duty <= (840000-40000) && duty >= (840000 - 84000)) {
		TC0->TC_CHANNEL[0].TC_RA = TC_RA_RA(duty);
		} else if(duty > (840000-40000)) {
		TC0->TC_CHANNEL[0].TC_RA = TC_RA_RA(840000-40000);
		} else if(duty < (840000-84000)) {
		TC0->TC_CHANNEL[0].TC_RA = TC_RA_RA(840000-84000);
	}
}

