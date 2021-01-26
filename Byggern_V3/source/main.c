#include <avr/io.h>
#include <avr/pgmspace.h>

#define F_CPU 4915200

#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "xmem.h"
#include "joystick.h"
#include "OLED.h"
#include "menu.h"
#include "linkedList.h"
#include "spi.h"
#include "mcp2515.h"
#include "can.h"


int main(void) {
	FILE uartio = FDEV_SETUP_STREAM(UART_Transmit, UART_Recieve, _FDEV_SETUP_RW);
	FILE oledio = FDEV_SETUP_STREAM(OLED_write_char8, UART_Recieve, _FDEV_SETUP_RW);
	
	stdout = &uartio;
		
	UART_init();
	XMEM_init();
	OLED_init();
	//initializes mcp2515 and SPI
	can_init();
	menu_init();
	//initializes joystick and ADC
	joystick_init();
	OLED_reset();
	
	// Set all values on SRAM to 0
	OLED_clear_SRAM();
	while(joystick_get_joystick_button()) {
		OLED_print_SRAM("Press the joystick.", 3, 20, 5);
		OLED_circleBres(57, 28, 9);
		_delay_ms(33);
		OLED_read_SRAM();
	}
	_delay_ms(200);
	printf("Verification of UART\r\n");
	while(1) {		
		//_delay_ms(20);
		//printf("Epicalyx    ");
		//joystick_send_pos();
		menu_update();
		_delay_ms(33);
		OLED_read_SRAM();
		//_delay_ms(20);
		//printf("STATUS_send: %x\n\r", mcp2515_read_status());
		//_delay_ms(100);
		//can_receive(&msg);
		//for(unsigned int i = 0; i < msg.length; i++) {
			//printf("msg %d = %x \n\r", i, msg.data[i]);
		//}
		//_delay_ms(100);
		//printf("STATUS_rec: %x\n\r", mcp2515_read_status());
		//_delay_ms(100);
	}
	


	// Draws a rectangle of lines
	for(int i = 0; i < 50; i++){
		OLED_plotLine(0,i,30,i);
	}
	
	//JOY_POS_PERCENT joypos;
	//JOY_DIR dir;
	//uint8_t pos_slider;
	
	//char a;
	//while(1) {
		//if(!((1 << PD4) & PIND)) {
			//printf("You pressed me baby");
		//}
		//
		//
		////printf("%d  \n\r ", adc_read(0));
		////pos_slider = slider_getpos(2);
		////printf("%d \n\r",pos_slider);
		////i = PINB;
		////joypos = joystick_getpos_percent();
		////LED_pos(joypos.y/32, joypos.x/2);
		////OLED_write_char(0xFF);
		////OLED_reset();
		////dir = joystick_getdir();
		////OLED_pos(0, 0);
		////printf("x: %d  Y:%d  ", joypos.x, joypos.y);
		////_delay_ms(10);
		////fprintf(&uartio,"x: %d  Y:%d  \n\r", joypos.x, joypos.y);
		////printf("%d \n\r",dir);	
		////printf("Epicalyx ");
		//
	//}
}