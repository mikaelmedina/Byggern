/*
 * spi.h
 *
 * Created: 23.09.2020 17:38:15
 *  Author: thomabsk
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>
#include "mcp2515.h"

void spi_master_init(void);
void spi_master_transmit(uint8_t data);
uint8_t spi_master_receive(void);



#endif /* SPI_H_ */