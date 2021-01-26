#include "joystick.h"

#define DEADZONE		10
#define MAX_VAL_ADC		255
#define LEFT_BUTTON		PB0
#define RIGHT_BUTTON	PB1
#define JOYSTICK_BUTTON PD4

static JOY_POS neutral_pos;


void joystick_init(void) {
	//Setup of buttons
	PORTB |= (1 << LEFT_BUTTON) | (1 << RIGHT_BUTTON);
	DDRB &= ~(1 << LEFT_BUTTON) | (1 << RIGHT_BUTTON);
	PORTD |= (1 << JOYSTICK_BUTTON);
	DDRD &= ~(1 << JOYSTICK_BUTTON);
	
	adc_init();

	joystick_calibrate();
}

//Joystick must be in neutral position to calibrate
void joystick_calibrate(void) {
	neutral_pos.x = adc_read(JOYSTICK_X);
	neutral_pos.y = adc_read(JOYSTICK_Y);
}


//Output raw value from ADC
JOY_POS joystick_getpos(void) {
	JOY_POS pos;

	pos.x = adc_read(JOYSTICK_X);

	pos.y = adc_read(JOYSTICK_Y);
	
	return pos;
}


JOY_POS_PERCENT joystick_getpos_percent(void){
		
	JOY_POS_PERCENT pos_percent;
	JOY_POS pos = joystick_getpos();
	
	if (pos.x < (neutral_pos.x - DEADZONE)) {
		pos_percent.x = -(100 - ((pos.x * 100) / neutral_pos.x - DEADZONE));
	}
	else if (pos.x > (neutral_pos.x + DEADZONE)) {
		pos_percent.x = ((pos.x - neutral_pos.x - DEADZONE) * 100) / (MAX_VAL_ADC - neutral_pos.x - DEADZONE);
	}
	else {
		pos_percent.x = 0;
	}

	if(pos.y < (neutral_pos.y - DEADZONE)){
		pos_percent.y = - (100 - ((pos.y * 100) / neutral_pos.y - DEADZONE));
	}
	else if(pos.y > (neutral_pos.y + DEADZONE)){
		pos_percent.y = ((pos.y - neutral_pos.y - DEADZONE) * 100)/ (MAX_VAL_ADC- neutral_pos.y - DEADZONE);
	}
	else{
		pos_percent.y = 0;
	}
		
	return pos_percent;
}


JOY_DIR joystick_getdir(void) {
	JOY_POS_PERCENT pos_percent = joystick_getpos_percent();	
	double angle = atan2(pos_percent.y,pos_percent.x);
	double angle_deg = (angle/M_PI) * 180;

	if(pos_percent.x == 0 && pos_percent.y == 0){
		return NEUTRAL;
	}
	else if(angle_deg >= 45 && angle_deg <135){
		return UP;
	}
	else if(angle_deg >= 135 || angle_deg <= -135){
		return LEFT;
	}
	else if(angle_deg > -135 && angle_deg<=-45){
		return DOWN;
	}
	else if(angle_deg >-45 && angle_deg < 45){
		return RIGHT;
	}
	else{
		return NEUTRAL;
	}
}


void joystick_send_pos(void){
	JOY_POS_PERCENT pos_percent = joystick_getpos_percent();
	JOY_DIR dir = joystick_getdir();
	int8_t right_button = joystick_get_right_button();
	uint8_t slider_pos = slider_getpos(SLIDER_RIGHT);
	CAN_MSG msg;
	
	msg.id = MSG_ID_JOYSTICK;
	msg.data[0] = pos_percent.x;
	msg.data[1] = pos_percent.y;
	msg.data[2] = dir;
	msg.data[3] = right_button;
	msg.data[4] = slider_pos;
	msg.length = 5;

	can_send(&msg);
}

//Warning: Only use adc_channels assigned to sliders
uint8_t slider_getpos(ADC_CHANNEL slider_channel) {
	uint8_t pos;
	pos = adc_read(slider_channel);
	return pos;
}

int8_t joystick_get_right_button(void) {
	int8_t right_button = ((1 << RIGHT_BUTTON) & PINB);

	return right_button;
}

int8_t joystick_get_left_button(void) {
	int8_t left_button = ((1 << LEFT_BUTTON) & PINB);

	return left_button;
}

int8_t joystick_get_joystick_button(void) {
	int8_t joystick_button = ((1 << JOYSTICK_BUTTON) & PIND);

	return joystick_button;
}