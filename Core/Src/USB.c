#include "fatfs.h"
#include "usb_host.h"
#include "USB.h"

extern ApplicationTypeDef Appli_state;
extern char USBHPath[4];
uint8_t mounted = 0;
uint8_t ready = 0;

static FATFS fs;
DIR current_dir;
FILINFO current_file_info;
FIL current_file;

int USB_read_current_file(uint8_t *buffer, int buffer_size){
	uint bytes_read;
	f_read(&current_file, buffer, buffer_size, &bytes_read);
	return buffer_size - bytes_read;
}

void USB_open_next_file(void){
	f_close(&current_file);
	f_readdir(&current_dir, &current_file_info);
	if(current_file_info.fname == NULL){
		f_readdir(&current_dir, NULL);
		f_readdir(&current_dir, &current_file_info);
	}
	f_open(&current_file, current_file_info.fname, FA_READ);
}

void USB_open_dir(void){
	f_opendir(&current_dir, "/");
}

void USB_handle_state(void){
    switch(Appli_state){
    	case APPLICATION_IDLE:
    		break;
    	case APPLICATION_START:
    		if(!mounted && f_mount(&fs, (TCHAR const *)USBHPath, 0) == FR_OK){
        		mounted = 1;
    			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    		}
    		break;
    	case APPLICATION_READY:
    		USB_open_dir();
    		ready = 1;
    		break;
    	case APPLICATION_DISCONNECT:
    		mounted = ready = 0;
    		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    		break;
    }
}

