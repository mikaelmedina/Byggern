#include "OLED.h"
#include "fonts.h"
#include <string.h>

#define BASE_ADDRESS 0x1000
#define BASE_DATA_ADDRESS 0x1200

void OLED_init() {
	volatile char* oled_com = (char*) BASE_ADDRESS;
	oled_com[0] = 0xAE; // Display off
	oled_com[0] = 0xA1; // Segment remap, A1 => column address 127 is mapped to SEG0
	oled_com[0] = 0xDA; // Common pads hardware: 
	oled_com[0] = 0x12;
	oled_com[0] = 0xC8; // Common output scan direction: com63-com0
	oled_com[0] = 0xA8; // Multiplex ration mode: 63
	oled_com[0] = 0x3F;
	oled_com[0] = 0xD5; // Display divide ratio/osc. freq. mode
	oled_com[0] = 0x80;
	oled_com[0] = 0x81; // Contrast control
	oled_com[0] = 0x50; 
	oled_com[0] = 0xD9; // Set pre-charge periode
	oled_com[0] = 0x21;
	
	oled_com[0] = 0x20; // Memory addressing mode: Horizontal addressing
	oled_com[0] = 0x00;
	
	oled_com[0] = 0x22; // Set page start and end
	oled_com[0] = 0x00;
	oled_com[0] = 0x07;
	
	oled_com[0] = 0x21; // Set column start and end
	oled_com[0] = 0x00;
	
	oled_com[0] = 127;
	
	oled_com[0] = 0xDB; // VCOM deselect level mode
	oled_com[0] = 0x30;
	oled_com[0] = 0xAD; // Master config, external Iref
	oled_com[0] = 0x00;
	oled_com[0] = 0xA4; // Out follows RAM content, display on
	oled_com[0] = 0xA6; // Normal display (contrary to inverse, A7)
	oled_com[0] = 0xAF; // Display on
}

void OLED_reset() {
	OLED_pos(0, 0);
	for(int i = 0; i < (9*128); i++) {
		OLED_write_data(0);
	}
	OLED_pos(0, 0);
}




void OLED_write_data(uint8_t data) {
	volatile char* oled_data = (char*) BASE_DATA_ADDRESS;
	oled_data[0] = data;
}

void OLED_write_char(char data, uint8_t font_size) {
	switch(font_size) {
		case(4):
			for(uint8_t j = 0; j < 4; j++){
				OLED_write_data(pgm_read_byte(&(font4[data-32][j])));
			}
			break;
		case(5):
			for(uint8_t j = 0; j < 5; j++){
				OLED_write_data(pgm_read_byte(&(font5[data-32][j])));
			}
			break;
		case(8):
			for(uint8_t j = 0; j < 8; j++){
				OLED_write_data(pgm_read_byte(&(font8[data-32][j])));
			}
			break;
	}
}

void OLED_write_char8(char data) {
	for(uint8_t j = 0; j < 8; j++){
		OLED_write_data(pgm_read_byte(&(font8[data-32][j])));
	}
}

void OLED_command(uint8_t command) {
	volatile char* oled_com = (char*) BASE_ADDRESS;
	oled_com[0] = command;
}

void OLED_pos(uint8_t row, uint8_t column) {
	OLED_command(0x22); // Page start and end
	OLED_command(row);
	OLED_command(7);
	
	OLED_command(0x21); // Column start and end
	OLED_command(column);
	OLED_command(127);
}

void OLED_draw_arrow(uint8_t row, uint8_t col){
	OLED_pos (row , col );
	OLED_write_data(0b00011000);
	OLED_write_data(0b00111100);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00011000);	
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
}

void OLED_read_SRAM(void) {
	OLED_pos(0, 0);
	volatile char* sram = (char*) 0x1800;
	char sram_val;
	for(unsigned int i = 0; i < 1024; i++) {
		sram_val = sram[i];
		OLED_write_data(sram_val);
	}
}

void OLED_write_SRAM_char(uint8_t data, uint16_t row, uint16_t col, uint8_t font_size) {
	uint16_t address = OLED_pos_SRAM(row, col);
	switch(font_size) {
		case(4):
			for(uint8_t j = 0; j < 4; j++){
				XMEM_write(pgm_read_byte(&(font4[data-32][j])), address);
				address++;
			}
			break;
		case(5):
			for(uint8_t j = 0; j < 5; j++){
				XMEM_write(pgm_read_byte(&(font5[data-32][j])), address);
				address++;
			}
			break;
		case(8):
			for(uint8_t j = 0; j < 8; j++){
				XMEM_write(pgm_read_byte(&(font8[data-32][j])), address);
				address++;
			}
		break;
	}
}

uint16_t OLED_pos_SRAM(uint16_t row, uint16_t col) {
	return (row*128 + col);
}

void OLED_print_SRAM(char* str, uint16_t row, uint16_t col, uint8_t font_size) {
	for(uint8_t i = 0; i < strlen(str); i++) {
		OLED_write_SRAM_char(str[i], row, col, font_size);
		col+=font_size;
	}
}

void OLED_clear_SRAM(void) {
	for(unsigned int i = 0; i < 1024; i++) {
		XMEM_write(0, i);
	}
}

 void OLED_set_pixel(int x, int y, int value){
	 int row_pixel = y % 8;
	 int row = y / 8;
	 int col = x;
	 char data = value << row_pixel;
	 int address = OLED_pos_SRAM(row,col);
	 char retrieved_data = XMEM_read(address);
	 retrieved_data &= ~(data);
	 data |= retrieved_data;
	 
	 XMEM_write(data, address);
 }
 
 static void plotLineLow(int x0,int y0,int x1,int y1){
	 int dx = x1-x0;
	 int dy = y1-y0;
	 int yi = 1;
	 if(dy<0){
		 yi = -1;
		 dy = -dy;
	 }
	 int D = (2*dy) - dx;
	 int y = y0;
	 
	 for(int x = x0;x<x1;x++){
		 OLED_set_pixel(x,y,1);
		 if(D>0){
			 y = y+yi;
			 D = D+ (2*(dy-dx));
		 }
		 else{
			 D = D+2*dy;
		 }
	 }
 }
 
 static void plotLineHigh(int x0,int y0,int x1,int y1){
	 int dx = x1-x0;
	 int dy = y1-y0;
	 int xi = 1;
	 if(dx<0){
		 xi = -1;
		 dx = -dx;
	 }
	 int D = (2*dx) - dy;
	 int x = x0;
	 
	 for(int y = y0;y<x1;y++){
		 OLED_set_pixel(x,y,1);
		 if(D>0){
			 x = x+xi;
			 D = D+ (2*(dx-dy));
		 }
		 else{
			 D = D+2*dx;
		 }
	 }
 }
 // using Bresenham's algorithm
 void OLED_plotLine(int x0,int y0, int x1, int y1){
	 if(abs(y1-y0) < abs(x1-x0)){
		 if(x0>x1){
			 plotLineLow(x1,y1,x0,y0);
		 }
		 else{
			 plotLineLow(x0,y0,x1,y1);
		 }
	 }
	 else{
		 if(y0>y1){
			 plotLineHigh(x1,y1,x0,y0);
		 }
		 else{
			 plotLineHigh(x0,y0,x1,y1);
		 }
	 }
 }
 
 void OLED_draw_Racket(int x, int y){
	 OLED_plotLine(x+9,y+0,x+11,y+0);
	 OLED_plotLine(x+6,y+1,x+14,y+1);
	 OLED_plotLine(x+5,y+2,x+15,y+2);
	 OLED_plotLine(x+4,y+3,x+16,y+3);
	 OLED_plotLine(x+4,y+4,x+16,y+4);
	 OLED_plotLine(x+4,y+5,x+16,y+5);
	 OLED_plotLine(x+4,y+6,x+16,y+6);
	 OLED_plotLine(x+5,y+7,x+15,y+7);
	 OLED_plotLine(x+6,y+8,x+14,y+8);
	 OLED_plotLine(x+9,y+9,x+11,y+9);
	 OLED_plotLine(x+9,y+10,x+11,y+10);
	 OLED_plotLine(x+9,y+11,x+11,y+11);
	 OLED_plotLine(x+9,y+12,x+11,y+12);
 }

// using Bresenham's algorithm
void OLED_drawCircle(int xc, int yc, int x, int y)
{
	OLED_set_pixel(xc+x, yc+y, 1);
	OLED_set_pixel(xc-x, yc+y, 1);
	OLED_set_pixel(xc+x, yc-y, 1);
	OLED_set_pixel(xc-x, yc-y, 1);
	OLED_set_pixel(xc+y, yc+x, 1);
	OLED_set_pixel(xc-y, yc+x, 1);
	OLED_set_pixel(xc+y, yc-x, 1);
	OLED_set_pixel(xc-y, yc-x, 1);
}

// using Bresenham's algorithm
void OLED_circleBres(int xc, int yc, int r)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	OLED_drawCircle(xc, yc, x, y);
	while (y >= x)
	{
		
		x++;
		
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
		d = d + 4 * x + 6;
		OLED_drawCircle(xc, yc, x, y);
	}
}
 