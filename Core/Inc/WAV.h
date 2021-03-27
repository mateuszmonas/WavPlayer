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
	WAV_PLAYBACK_STATE_IDLE=0,
	WAV_PLAYBACK_STATE_NEXT,
	WAV_PLAYBACK_STATE_RESET
}WAV_PLAYBACK_STATE;

typedef enum{
	WAV_BUFFER_STATE_HALF_BUFFER=0,
	WAV_BUFFER_STATE_FULL_BUFFER,
	WAV_BUFFER_STATE_IDLE
}WAV_BUFFER_STATE;

void WAV_init(void);
void WAV_start_play(void);
void WAV_process(void);
void WAV_next_song(void);
void WAV_reset_songs(void);
void WAV_stop_play(void);
uint8_t WAV_is_running(void);
