/*
 * can.h
 *
 * Created: 30.09.2020 11:24:13
 *  Author: thomabsk
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "mcp2515.h"

#define MSG_ID_JOYSTICK			0x0002
#define MSG_ID_GAME_STATUS		0x0003
#define MSG_ID_POINT_SCORED		0x0004

typedef struct CAN_MSG {
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
}CAN_MSG;

int get_score_flag(void);
void set_score_flag(int score);

void can_init(void);
void can_send(CAN_MSG* message);
void can_receive(CAN_MSG* message);
uint8_t can_transmit_done(uint8_t transmit_buffer);

#endif /* CAN_H_ */