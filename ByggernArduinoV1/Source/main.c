/*
 * ByggernArduinoV1.c
 *
 * Created: 07.10.2020 09:00:43
 */ 


#include "sam.h"
#include "sam3x8e.h"
#include "uart.h"
#include "can_controller.h"
#include "printf-stdarg.h"
#include "can_interrupt.h"
#include "game_controller.h"

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	
	__NVIC_EnableIRQ(ADC_IRQn);
	//__NVIC_EnableIRQ(TC1_IRQn);
	
	configure_uart();
	can_init(0x00290561, 2, 3);
		
	game_controller_init();
	
	printf("TEST");
	CAN_MESSAGE msg;
	msg.id = 0x0000;
	msg.data[0] = 0xAA;
	msg.data[1] = 0xAA;
	msg.data_length = 2;
	
	can_send(&msg, 0);
	
	for(int i = 0; i < 10000000; i++);
	msg.data[2] = 0xFF;
	msg.data_length = 3;
	can_send(&msg, 0);
	
	
	game_controller_run_game();
}
