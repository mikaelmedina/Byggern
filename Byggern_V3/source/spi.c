#include "spi.h"

void spi_master_init(void) {
	DDRB |= (1 << DDB5)|(1 << DDB7);
	SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPR0);
}

void spi_master_transmit(uint8_t data) {
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
}

uint8_t spi_master_receive(void) {
	uint8_t data = SPDR;
	return data;
}