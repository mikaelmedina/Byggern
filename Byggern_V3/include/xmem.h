#ifndef XMEM_H_
#define XMEM_H_

#include <stdint.h>

void XMEM_init(void);
void XMEM_write(uint8_t data, uint16_t address);
void SRAM_test(void);
uint8_t XMEM_read(uint16_t address);


#endif /* XMEM_H_ */