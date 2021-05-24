#include "stm32f4xx_hal.h"

void MP45DT02_init(void);
void MP45DT02_start(uint16_t* audio_buffer, uint32_t len);
void MP45DT02_stop(void);
void MP45DT02_half_buffer_callback(void);
void MP45DT02_full_buffer_callback(void);
