#include "WAV.h"
#include "CS43L22.h"
#include "USB.h"

#include "stm32f4xx_hal.h"
uint8_t audio_buffer[WAV_BUFFER_SIZE];

WAV_PLAYBACK_STATE current_playback_state = WAV_PLAYBACK_STATE_IDLE;
WAV_BUFFER_STATE current_buffer_state = WAV_BUFFER_STATE_IDLE;

uint8_t running = 0;

void WAV_init(I2C_HandleTypeDef hi2c){
	CS43L22_init();
}

void WAV_start_play(void){
	static WAV_HeaderTypeDef header;
	static CS43L22_DataFormatTypeDef data_format;
	USB_open_next_file();
	USB_read_current_file((uint8_t *)&header, sizeof(header));
	USB_read_current_file(audio_buffer, WAV_BUFFER_SIZE);

	data_format.bit_per_sample = header.bit_per_sample;
	data_format.num_channels = header.num_channels;
	data_format.sample_rate = header.sample_rate;
	CS43L22_start((uint16_t *)audio_buffer, WAV_BUFFER_SIZE / 2, data_format);
	running = 1;
}

void WAV_process(void){
	if(!USB_mounted()) return;
	switch(current_playback_state){
		case WAV_PLAYBACK_STATE_NEXT:
			WAV_stop_play();
			WAV_start_play();
			break;
		case WAV_PLAYBACK_STATE_RESET:
			WAV_stop_play();
			USB_rewind_files();
			current_playback_state = WAV_PLAYBACK_STATE_NEXT;
			break;
		case WAV_PLAYBACK_STATE_IDLE:
		default:
			break;
	}
	switch(current_buffer_state){
		case WAV_BUFFER_STATE_HALF_BUFFER:
			current_buffer_state = WAV_BUFFER_STATE_IDLE;
			if(USB_read_current_file(audio_buffer, WAV_BUFFER_SIZE / 2)){
				current_playback_state = WAV_PLAYBACK_STATE_NEXT;
			}
			break;
		case WAV_BUFFER_STATE_FULL_BUFFER:
			current_buffer_state = WAV_BUFFER_STATE_IDLE;
			if(USB_read_current_file(audio_buffer + WAV_BUFFER_SIZE / 2, WAV_BUFFER_SIZE / 2)){
				current_playback_state = WAV_PLAYBACK_STATE_NEXT;
			}
			break;
		case WAV_BUFFER_STATE_IDLE:
		default:
			break;
	}
}

uint8_t WAV_is_running(void){
	return running;
}

void WAV_next_song(void){
	current_playback_state = WAV_PLAYBACK_STATE_NEXT;
}

void WAV_reset_songs(void){
	current_playback_state = WAV_PLAYBACK_STATE_RESET;
}

void WAV_stop_play(void){
	CS43L22_stop();
	running = 0;
	current_playback_state = WAV_PLAYBACK_STATE_IDLE;
	current_buffer_state = WAV_BUFFER_STATE_IDLE;
}

void CS43L22_half_buffer_callback(void){
	current_buffer_state = WAV_BUFFER_STATE_HALF_BUFFER;
}

void CS43L22_half_full_callback(void){
	current_buffer_state = WAV_BUFFER_STATE_FULL_BUFFER;
}

void USB_mounted_callback(void){
	WAV_start_play();
}

void USB_unmounted_callback(void){
	WAV_stop_play();
}
