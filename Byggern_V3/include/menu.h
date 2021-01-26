/*
 * menu.h
 *
 * Created: 16.09.2020 17:30:25
 *  Author: thomabsk
 */ 


#ifndef MENU_H_
#define MENU_H_
#include <avr/io.h>

#define F_CPU 4915200
#include <util/delay.h>

#include "joystick.h"
#include "OLED.h"
#include "linkedList.h"
#include "can.h"

void menu_init(void);
void menu_update(void);

typedef enum{
	JOYSTICK,
	SLIDER
	}CONTROLLER_MODE;

#endif /* MENU_H_ */