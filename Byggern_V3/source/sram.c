#include "xmem.h"
#include "avr/io.h"

#define BASE_ADDRESS 0x1000

void XMEM_init(void) {
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
}

void XMEM_write(uint8_t data, uint16_t address) {
	volatile char *ext_mem = (char * ) BASE_ADDRESS;
	ext_mem[address] = data;
}
