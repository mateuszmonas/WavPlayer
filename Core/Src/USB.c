#include "fatfs.h"
#include "usb_host.h"
#include "USB.h"

extern char USBHPath[4];

FATFS fs;
DIR current_reading_dir;
DIR current_writing_dir;
FILINFO current_reading_file_info;
FILINFO current_writing_file_info;
FIL current_reading_file;
FIL current_writing_file;
uint8_t mounted = 0;

int USB_read_current_file(uint8_t *buffer, uint32_t buffer_size){
	uint bytes_read;
	f_read(&current_reading_file, buffer, buffer_size, &bytes_read);
	return buffer_size - bytes_read;
}

void USB_skip_directories(){
	while(current_reading_file_info.fname[0] == 0 || (current_reading_file_info.fattrib & AM_DIR)){
		if(current_reading_file_info.fname[0] == 0){
			f_readdir(&current_reading_dir, NULL);
			f_readdir(&current_reading_dir, &current_reading_file_info);
		} else if(current_reading_file_info.fattrib & AM_DIR){
			f_readdir(&current_reading_dir, &current_reading_file_info);
		}
	}
}

void USB_rewind_files(void){
	f_close(&current_reading_file);
	f_readdir(&current_reading_dir, NULL);
	USB_skip_directories();
	f_open(&current_reading_file, current_reading_file_info.fname, FA_READ);
}

void USB_open_next_file(void){
	f_close(&current_reading_file);
	f_readdir(&current_reading_dir, &current_reading_file_info);
	USB_skip_directories();
	f_open(&current_reading_file, current_reading_file_info.fname, FA_READ);
}

void USB_create_wav_file_for_writing(void){
	FRESULT result;
	f_close(&current_reading_file);
	while((result = f_open(&current_writing_file, "recording.wav", FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK) {
		int error = f_error(&current_writing_file);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}

void USB_write_current_file(uint8_t *buffer, uint32_t buffer_size){
	uint bytes_written;
	FRESULT result;
	if((result = f_write(&current_writing_file, buffer, buffer_size, &bytes_written)) != FR_OK) {
		int error = f_error(&current_writing_file);
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	}
}

void USB_close_wav_file_for_writing(void){
	FRESULT result;
	if((result = f_close(&current_writing_file)) != FR_OK) {
		int error = f_error(&current_writing_file);
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	}
}

uint8_t USB_mounted(void){
	return mounted;
}

__weak void USB_mounted_callback(void){

}

__weak void USB_unmounted_callback(void){

}

void USB_handle_state(uint8_t state){
	switch(state){
      case HOST_USER_CONNECTION:
		  f_mount(&fs, (TCHAR const *)USBHPath, 0);
		  break;
      case HOST_USER_CLASS_ACTIVE:
    	  if(f_opendir(&current_reading_dir, "/") == FR_OK){
			  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
			  mounted = 1;
        	  USB_mounted_callback();
    	  }
      	  break;
	  case HOST_USER_DISCONNECTION:
		  mounted = 0;
		  f_mount(NULL, (TCHAR const *)"", 0);
		  USB_unmounted_callback();
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		  break;
	  default:
		  break;
	  }
}

