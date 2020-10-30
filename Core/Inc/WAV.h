#define WAV_BUFFER_SIZE 4096*2

#include "stdint.h"
#include "stm32f4xx_hal.h"

typedef struct
{
  uint32_t   chunk_id;
  uint32_t   chunk_size;
  uint32_t   format;
  uint32_t   sub_chunk_id;
  uint32_t   sub_chunk_1_size;
  uint16_t   audio_format;
  uint16_t   num_channels;
  uint32_t   sample_rate;

  uint32_t   byte_rate;
  uint16_t   block_align;
  uint16_t   bit_per_sample;
  uint32_t   sub_chunk_2_id;
  uint32_t   sub_chunk_2_size;

}WAV_HeaderTypeDef;

typedef enum{
	WAV_BUFFER_STATE_HALF_BUFFER=0,
	WAV_BUFFER_STATE_FULL_BUFFER,
	WAV_BUFFER_STATE_IDLE
}WAF_BUFFER_STATE;

typedef enum{
	WAV_PLAYBACK_STATE_IDLE=0,
	WAV_PLAYBACK_STATE_NEXT,
	WAV_PLAYBACK_STATE_RESET
}WAF_PLAYBACK_STATE;

void init(I2C_HandleTypeDef hi2c);
void start_play(void);
void process(void);
void next_song(void);
void reset_songs(void);
void stop_play(void);
uint8_t is_running(void);
