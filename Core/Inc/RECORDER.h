#define RECORDER_BUFFER_SIZE 16 * 8
#define PCM_BUFFER_SIZE 16 * 64

#include "stm32f4xx_hal.h"

typedef enum{
	RECORDER_BUFFER_STATE_HALF_BUFFER=0,
	RECORDER_BUFFER_STATE_FULL_BUFFER,
	RECORDER_BUFFER_STATE_IDLE
}RECORDER_BUFFER_STATE;

void RECORDER_process(void);
void RECORDER_start(void);
void RECORDER_stop(void);
uint8_t RECORDER_is_running(void);
