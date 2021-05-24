#include "MP45DT02.h"

static I2S_HandleTypeDef hi2sx;

void MP45DT02_init(void){
	  hi2sx.Instance = SPI2;
	  hi2sx.Init.Mode = I2S_MODE_MASTER_RX;
	  hi2sx.Init.Standard = I2S_STANDARD_PHILIPS;
	  hi2sx.Init.DataFormat = I2S_DATAFORMAT_16B;
	  hi2sx.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	  hi2sx.Init.AudioFreq = I2S_AUDIOFREQ_8K;
	  hi2sx.Init.CPOL = I2S_CPOL_LOW;
	  hi2sx.Init.ClockSource = I2S_CLOCK_PLL;
	  hi2sx.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
	  HAL_I2S_Init(&hi2sx);
}

void MP45DT02_start(uint16_t* audio_buffer, uint32_t len){
	if(HAL_I2S_Receive_DMA(&hi2sx, audio_buffer, len) == HAL_OK){
	    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}
}

void MP45DT02_stop(void){
	if(HAL_I2S_DMAStop(&hi2sx) == HAL_OK){
	    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	}
}

__weak void MP45DT02_half_buffer_callback(void){

}

__weak void MP45DT02_full_buffer_callback(void){

}
