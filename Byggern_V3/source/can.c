/*
 * can.c
 *
 * Created: 30.09.2020 11:23:59
 *  Author: thomabsk
 */ 

#include "can.h"

static int score_flag = 0;

int get_score_flag(void) {
	return score_flag;
}

void set_score_flag(int score) {
	score_flag = score;
}

ISR(INT0_vect) {
	uint8_t flags = mcp2515_read(MCP_CANINTF) & 0x03;
	CAN_MSG msg;
	
	for(int i = 0; i < 3; i++) {
		if(mcp2515_read(MCP_CANINTF) & (1 << (2 + i))) {
			//printf("Transmit interrupt: %x\n\r", mcp2515_read(MCP_CANINTF) & (1 << (2 + i)));
			mcp2515_bit_modify(MCP_CANINTF, (1 << (2 + i)), 0x00);
		}
	}
	
	if(flags) {
		can_receive(&msg);
		if(msg.id == MSG_ID_POINT_SCORED) {
			set_score_flag(1);
		}
		for(unsigned int i = 0; i < msg.length; i++) {
			printf("Msg %d = %x, ID: %x\n\r", i, msg.data[i], msg.id);
		}
		
		mcp2515_bit_modify(MCP_CANINTF, 0x03, 0x00);
	}
}

void can_init(void) {
	mcp2515_init();
	
	mcp2515_bit_modify(MCP_CNF1, 0xFF, 0x43);
	mcp2515_bit_modify(MCP_CNF2, 0xFF, 0xB5);
	mcp2515_bit_modify(MCP_CNF3, 0xFF, 1);

	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	
	uint8_t can_status;
	can_status = mcp2515_read(MCP_CANSTAT);
	if((can_status & MODE_MASK) != MODE_NORMAL) {
		printf("MCP2515 is NOT in normal mode.\n\r");
	}
	
	// Disable interrupts
	cli();
	// External interrupt request 0 enable
	GICR |= (1 << INT0);
	// Enable global interrupts
	sei();
	// Falling edge generates interrupt
	MCUCR |= (1 << ISC01);
	
	// Receive buffer 0 control register
	mcp2515_bit_modify(MCP_RXB0CTRL, 0b01100000, 0xFF);
	
	// Transmit buffer 0 control register
	mcp2515_bit_modify(MCP_TXB0CTRL, 0b00000011, 0xFF);
	// Transmit buffer 1 control register
	mcp2515_bit_modify(0x40, 0b00000011, 0xFF);
	// Transmit buffer 2 control register
	mcp2515_bit_modify(0x50, 0b00000011, 0xFF);
}

void can_send(CAN_MSG* message) {
	static uint8_t buffer = 0;
	
	while(!can_transmit_done(buffer)) {
		buffer++;
		if(buffer > 2) {
			buffer = 0;
		}
	}

	uint8_t id_high = (uint8_t) (message->id >> 3);
	uint8_t id_low = (uint8_t)((message->id) << 5);

	uint8_t length = message->length;
	
	
	mcp2515_write(id_high, MCP_TXB0SIDH + 0x10 * buffer);
	mcp2515_write(id_low, MCP_TXB0SIDL + 0x10 * buffer);
	mcp2515_write(length, MCP_TXB0DLC + 0x10 * buffer);
	
	for(unsigned int i = 0; i < length; i++) {
		mcp2515_write(message->data[i], (MCP_TXB0DB0 + 0x10 * buffer)+i);
	}
	
	mcp2515_request_to_send(buffer);
}

void can_receive(CAN_MSG* message) {
	message->length = (0x0F & mcp2515_read(MCP_RXB0DLC));
	message->id = ((((uint16_t) mcp2515_read(MCP_RXB0SIDH)) << 3) | (mcp2515_read(MCP_RXB0SIDL) >> 5));
	
	for(unsigned int i = 0; i < message->length; i++) {
		message->data[i] = mcp2515_read(MCP_RXB0DB0+i);
	}
	mcp2515_bit_modify(MCP_CANINTF, (1 << 0) ,0);
}

uint8_t can_transmit_done(uint8_t transmit_buffer) {
	uint8_t i_flags = (mcp2515_read(MCP_CANINTF) & (1 << (2 + transmit_buffer)));
	return !i_flags;
}