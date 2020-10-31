#include "fatfs.h"
#include "usb_host.h"
#include "USB.h"

extern char USBHPath[4];

FATFS fs;
DIR current_dir;
FILINFO current_file_info;
FIL current_file;
uint8_t mounted = 0;

int USB_read_current_file(uint8_t *buffer, uint32_t buffer_size){
	uint bytes_read;
	f_read(&current_file, buffer, buffer_size, &bytes_read);
	return buffer_size - bytes_read;
}

void USB_skip_directories(){
	while(current_file_info.fname[0] == 0 || (current_file_info.fattrib & AM_DIR)){
		if(current_file_info.fname[0] == 0){
			f_readdir(&current_dir, NULL);
			f_readdir(&current_dir, &current_file_info);
		} else if(current_file_info.fattrib & AM_DIR){
			f_readdir(&current_dir, &current_file_info);
		}
	}
}

void USB_rewind_files(void){
	f_close(&current_file);
	f_readdir(&current_dir, NULL);
	USB_skip_directories();
	f_open(&current_file, current_file_info.fname, FA_READ);
}

void USB_open_next_file(void){
	f_close(&current_file);
	f_readdir(&current_dir, &current_file_info);
	USB_skip_directories();
	f_open(&current_file, current_file_info.fname, FA_READ);
}

uint8_t USB_mounted(){
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
    	  if(f_opendir(&current_dir, "/") == FR_OK){
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

