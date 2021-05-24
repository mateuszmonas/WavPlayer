#include "MP45DT02.h"
#include "USB.h"
#include "RECORDER.h"
#include "pdm2pcm.h"
#include "WAV.h"

static uint8_t pdm_buffer[RECORDER_BUFFER_SIZE];
static uint8_t pcm_buffer[PCM_BUFFER_SIZE];

static RECORDER_BUFFER_STATE current_buffer_state = RECORDER_BUFFER_STATE_IDLE;
static uint8_t is_running = 0;

void pdm_to_pcm(int current_pdm_buffer_position){
	static unsigned int current_pcm_buffer_position = 0;
	PDM_Filter(pdm_buffer + current_pdm_buffer_position, pcm_buffer + current_pcm_buffer_position , &PDM1_filter_handler);
	current_pcm_buffer_position += 32;
	if(current_pcm_buffer_position >= PCM_BUFFER_SIZE){
		USB_write_current_file(pcm_buffer, PCM_BUFFER_SIZE);
		current_pcm_buffer_position = 0;
	}
}

void RECORDER_process(void){
	if(!USB_mounted()) return;
	switch(current_buffer_state) {
		case RECORDER_BUFFER_STATE_HALF_BUFFER:
			current_buffer_state = RECORDER_BUFFER_STATE_IDLE;
			pdm_to_pcm(0);
			break;
		case RECORDER_BUFFER_STATE_FULL_BUFFER:
			current_buffer_state = RECORDER_BUFFER_STATE_IDLE;
			pdm_to_pcm(RECORDER_BUFFER_SIZE / 2);
			break;
		case RECORDER_BUFFER_STATE_IDLE:
		default:
			break;
	}
}

WAV_HeaderTypeDef RECORDER_create_header(void){
	WAV_HeaderTypeDef header;
	header.chunk_id = 0x52 | (0x49 << 8) | (0x46 << 16) | (0x46 << 24);
	header.format = 0x57 | (0x41 << 8) | (0x56 << 16) | (0x45 << 24);
	header.sub_chunk_id = 0x66 | (0x6d << 8) | (0x74 << 16) | (0x20 << 24);
	header.sub_chunk_1_size = 16;
	header.audio_format = 1;
	header.num_channels = PDM1_filter_handler.out_ptr_channels;
	header.sample_rate = I2S_AUDIOFREQ_8K;
	header.bit_per_sample = 16;
	header.byte_rate = header.num_channels * header.sample_rate * header.bit_per_sample / 8;
	header.block_align = header.num_channels * header.bit_per_sample / 8;
	header.sub_chunk_2_id= 0x64 | (0x61 << 8) | (0x74 << 16) | (0x61 << 24);
	header.sub_chunk_2_size = 0;
	header.chunk_size = 0;
	return header;
}


void RECORDER_start(void){
	is_running = 1;
	USB_create_wav_file_for_writing();
	WAV_HeaderTypeDef header = RECORDER_create_header();
	USB_write_current_file((uint8_t *)&header, sizeof(header));
	MP45DT02_start((uint16_t *)pdm_buffer, RECORDER_BUFFER_SIZE / 2);
}

uint8_t RECORDER_is_running(void){
	return is_running;
}

void RECORDER_stop(void){
	MP45DT02_stop();
	current_buffer_state = RECORDER_BUFFER_STATE_IDLE;
	USB_close_wav_file_for_writing();
	is_running = 0;
}

void MP45DT02_half_buffer_callback(void){
	current_buffer_state = RECORDER_BUFFER_STATE_HALF_BUFFER;
}

void MP45DT02_full_buffer_callback(void){
	current_buffer_state = RECORDER_BUFFER_STATE_FULL_BUFFER;
}
