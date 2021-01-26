/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. Jølsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_

#include <stdio.h>
#include <stdint.h>
#include "sam.h"
#include "printf-stdarg.h"
#include "game_controller.h"
#include "can_controller.h"

#define MSG_ID_JOYSTICK			0x0002
#define MSG_ID_GAME_STATUS		0x0003
#define MSG_ID_POINT_SCORED		0x0004

void CAN0_Handler       ( void );

#endif /* CAN_INTERRUPT_H_ */