/*
 * mcp2515.c
 *
 * Created: 30.09.2020 09:37:22
 *  Author: thomabsk
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "mcp2515.h"
#include "spi.h"

#define MCP2515_CS PB4


uint8_t mcp2515_init(void) {

	//Setup of CS
	DDRB = (1 << MCP2515_CS);
	PORTB |= (1 << MCP2515_CS);

	uint8_t can_status;
	
	spi_master_init();
	mcp2515_reset();
	
	can_status = mcp2515_read(MCP_CANSTAT);
	if((can_status & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is NOT in config mode.\n\r");
		return 1;
	}
	
	mcp2515_write(0x1D, MCP_CANINTE);
	
	return 0;
}

uint8_t mcp2515_read(uint8_t address) {
	PORTB &= ~(1 << MCP2515_CS);
	spi_master_transmit(MCP_READ);
	spi_master_transmit(address);
	spi_master_transmit(0x00);
	PORTB |= (1 << MCP2515_CS);
	uint8_t result = spi_master_receive();
	return result;
}

void mcp2515_write(uint8_t data, uint8_t address) {
	PORTB &= ~(1 << MCP2515_CS);
	spi_master_transmit(MCP_WRITE);
	spi_master_transmit(address);
	spi_master_transmit(data);
	PORTB |= (1 << MCP2515_CS);
}

void mcp2515_reset(void) {
	PORTB &= ~(1 << MCP2515_CS);
	spi_master_transmit(MCP_RESET);
	PORTB |= (1 << MCP2515_CS);
}

void mcp2515_request_to_send(uint8_t transmit_buffer) {
	PORTB &= ~(1 << MCP2515_CS);
	switch(transmit_buffer) {
		case(0):
			spi_master_transmit(MCP_RTS_TX0);
			break;
		case(1):
			spi_master_transmit(MCP_RTS_TX1);
			break;
		case(2):
			spi_master_transmit(MCP_RTS_TX2);
			break;
	}
	PORTB |= (1 << MCP2515_CS);
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	PORTB &= ~(1 << MCP2515_CS);
	spi_master_transmit(MCP_BITMOD);
	spi_master_transmit(address);
	spi_master_transmit(mask);
	spi_master_transmit(data);
	PORTB |= (1 << MCP2515_CS);
}

uint8_t mcp2515_read_status(void) {
	PORTB &= ~(1 << MCP2515_CS);
	spi_master_transmit(MCP_READ_STATUS);
	spi_master_transmit(0x00);
	spi_master_transmit(0x00);
	PORTB |= (1 << MCP2515_CS);
	uint8_t status = spi_master_receive();
	return status;
}
