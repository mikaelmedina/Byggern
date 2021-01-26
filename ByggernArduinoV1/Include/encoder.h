#ifndef ENCODER_H_
#define ENCODER_H_


#include "sam.h"
#include "sam3x8e.h"
#include <stdint.h>
#include "timer.h"

void encoder_init(void);
void encoder_reset(void);
int encoder_read(void);
int16_t encoder_get_encoder_value(void); //Always read encoder with encoder_read() before use


#endif /* ENCODER_H_ */