#include "menu.h"

static struct Node* currentNode = NULL;


static void menu_draw(void){
	OLED_clear_SRAM();
	OLED_print_SRAM(currentNode->prev->data, 0, 0, 4);
	OLED_print_SRAM(currentNode->data, 0, 43, 5);
	OLED_plotLine(43,8,43+8*5,8);
	OLED_print_SRAM(currentNode->next->data, 0, 95, 4);
	if(currentNode->child != NULL) {
		OLED_print_SRAM(currentNode->child->data, 2, 43, 4);
	}
	OLED_read_SRAM();
}


void menu_init() {
	struct Node* play = linkedList_createNode("  Play  ");
	struct Node* settings = linkedList_createNode("Settings");
	struct Node* credits = linkedList_createNode("Credits ");
	struct Node* play_joystick = linkedList_createNode("Joystick");
	struct Node* play_slider = linkedList_createNode("Slider ");
	
	
	play->next = settings;
	play->prev = credits;
	play->label = PLAY;
	play->child = play_joystick;
	play->parent = play_slider;
	
	play_joystick->label = PLAY_JOYSTICK;
	play_joystick->parent = play;
	play_joystick->child = play_slider;
	play_joystick->next = NULL;
	play_joystick->prev = NULL;
	
	play_slider->label = PLAY_SLIDER;
	play_slider->parent = play_joystick;
	play_slider->child = play;
	
	
	settings->next = credits;
	settings->prev = play;
	settings->label = SETTINGS;
	
	credits->next = play;
	credits->prev = settings;
	credits->label = CREDITS;
	credits->child = NULL;
	
	currentNode = play;
	
	// Sub-menus
	struct Node* setting_joycal = linkedList_createNode("Joycal");
	struct Node* setting_dummy = linkedList_createNode("Dummy");
	
	settings->child = setting_joycal;
	settings->parent = setting_joycal;
	
	setting_joycal->parent = settings;
	setting_joycal->child = settings;
	setting_joycal->next = setting_dummy;
	setting_joycal->prev = setting_dummy;
	setting_joycal->label = JOYCAL;
	
	setting_dummy->parent = settings;
	setting_dummy->child = settings;
	setting_dummy->next = setting_joycal;
	setting_dummy->prev = setting_joycal;
	setting_dummy->label = DUMMY;
}

void menu_update(void) {
	JOY_DIR joydir;
	joydir = joystick_getdir();
	_delay_us(10);
	if(!joystick_get_joystick_button()) {
		menu_select();
	}
	
	if(joydir != NEUTRAL) {
		menu_navigate(joydir);
		menu_draw();
		_delay_ms(500);
	}
	menu_draw();
}

void menu_navigate(JOY_DIR joydir) {
	switch(joydir) {
		case(LEFT):
			if(currentNode->prev != NULL) {
				currentNode = currentNode->prev;
			}
			break;
		case(RIGHT):
			if(currentNode->next != NULL) {
				currentNode = currentNode->next;
			}
			break;
		case(UP):
			if(currentNode->parent != NULL) {
				currentNode = currentNode->parent;
			}
			break;
		case(DOWN):
			if(currentNode->child != NULL) {
				currentNode = currentNode->child;
			}
			break;
	}
}

void menu_select(void) {
	switch(currentNode->label) {
		case(PLAY_JOYSTICK):
			menu_select_play(JOYSTICK);
			break;
		case(PLAY_SLIDER):
			menu_select_play(SLIDER);
			break;
		case(SETTINGS):
			break;
		case(CREDITS):
			menu_select_credits();
			break;
		case(JOYCAL):
			menu_select_joycal();
			break;
	}
}

void menu_select_play(CONTROLLER_MODE mode) {
	OLED_clear_SRAM();
	_delay_ms(200);
	
	// Send message to node 2 that the game is starting
	CAN_MSG msg_game_status;
	msg_game_status.data[0] = 0x1;
	msg_game_status.data[1] = mode;
	msg_game_status.length = 0x02;
	msg_game_status.id = MSG_ID_GAME_STATUS;
	can_send(&msg_game_status);

	JOY_POS pos = joystick_getpos();
	OLED_draw_Racket(50, 30);
	OLED_read_SRAM();
	int score = 0;
	char score_c[10];
	
	while(joystick_get_joystick_button()) {
		if(get_score_flag()) {
			score++;
			OLED_clear_SRAM();
			OLED_print_SRAM("Press to resume.", 0, 25, 5);
			OLED_print_SRAM("Points lost: ", 2, 30, 5);
			OLED_print_SRAM(itoa(score, score_c, 10), 2, 95, 5);
			OLED_read_SRAM();
			set_score_flag(0);

			while(joystick_get_joystick_button());

			_delay_ms(200);
			msg_game_status.data[0] = 0x1;
			can_send(&msg_game_status);
			OLED_clear_SRAM();
			OLED_draw_Racket(50, 30);
			OLED_read_SRAM();
		}
		//pos = joystick_getpos();
		//OLED_draw_Racket(pos.x/8, pos.y/8);
		//OLED_read_SRAM();
		//_delay_ms(33);
		//OLED_clear_SRAM();
		joystick_send_pos();
	}
	msg_game_status.data[0] = 0x0;
	can_send(&msg_game_status);
	_delay_ms(200);
}

void menu_select_credits(void) {
	OLED_clear_SRAM();
	OLED_print_SRAM("Team Epicalyx: ", 0, 0, 8);
	OLED_print_SRAM("Vegard Haraldstad", 1, 2, 5);
	OLED_print_SRAM("Thomas Borge", 2, 2, 5);
	OLED_print_SRAM("Mikael Medina", 3, 2, 5);
	OLED_plotLine(0, 7, 116, 7);
	OLED_read_SRAM();
	_delay_ms(200);
	while(joystick_get_joystick_button());
	_delay_ms(200);
}

void menu_select_joycal() {
	_delay_ms(200);
	OLED_clear_SRAM();
	OLED_print_SRAM("Please let go of the joystick", 0, 5, 4);
	OLED_read_SRAM();
	_delay_ms(1000);	
	for(int i = 20; i > 0; i--){
		if(i == 10) {joystick_calibrate();}
		OLED_circleBres(64,30,i);
		OLED_read_SRAM();
		_delay_ms(200);
	}
	OLED_print_SRAM("Joystick calibrated", 6, 25, 4);
	OLED_read_SRAM();
	_delay_ms(2000);
}