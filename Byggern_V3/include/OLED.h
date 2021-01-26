#ifndef OLED_H_
#define OLED_H_
#include <stdint.h>

void OLED_init(void);
void OLED_reset(void);
void OLED_write_data(uint8_t data);
void OLED_write_char(char data, uint8_t font_size);
void OLED_write_char8(char data);
void OLED_write_string(char* str, uint8_t font_size);
void OLED_command(uint8_t command);
void OLED_pos(uint8_t row, uint8_t column);
void OLED_draw_arrow(uint8_t row, uint8_t col);


void OLED_read_SRAM(void);
void OLED_write_SRAM_char(uint8_t data, uint16_t row, uint16_t col, uint8_t font_size);
uint16_t OLED_pos_SRAM(uint16_t row, uint16_t col);
void OLED_print_SRAM(char* str, uint16_t row, uint16_t col, uint8_t font_size);
void OLED_clear_SRAM(void);


void OLED_set_pixel(int x, int y, int value);
void OLED_plotLine(int x0,int y0, int x1, int y1);
void OLED_draw_Racket(int x, int y);
void OLED_circleBres(int xc, int yc, int r);

#endif /* OLED_H_ */