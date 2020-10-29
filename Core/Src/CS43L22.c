#include "CS43L22.h"

#include "stm32f4xx_hal.h"
#include "math.h"

static I2C_HandleTypeDef hi2cx;
static I2S_HandleTypeDef hi2sx;

void CS43L22_hi2cx_init(){
	hi2cx.Instance = I2C1;
	hi2cx.Init.ClockSpeed = 100000;
	hi2cx.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2cx.Init.OwnAddress1 = 0;
	hi2cx.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2cx.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2cx.Init.OwnAddress2 = 0;
	hi2cx.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2cx.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hi2cx);
}

void CS43L22_hi2sx_init(){
	hi2sx.Instance = SPI3;
	hi2sx.Init.Mode = I2S_MODE_MASTER_TX;
	hi2sx.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2sx.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2sx.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2sx.Init.AudioFreq = I2S_AUDIOFREQ_11K;
	hi2sx.Init.CPOL = I2S_CPOL_LOW;
	hi2sx.Init.ClockSource = I2S_CLOCK_PLL;
	HAL_I2S_Init(&hi2sx);
}

void CS43L22_init(){
	CS43L22_hi2cx_init();
	CS43L22_hi2sx_init();
	HAL_Delay(500);
	CS43L22_set_power(DOWN);
	CS43L22_write(CS43L22_REGISTRY_INTERFACE_CONRTOL_1, 0x07);
	CS43L22_enable_channel(LEFT_HEADPHONE & RIGHT_HEADPHONE);
	CS43L22_set_power(UP);
	HAL_GPIO_WritePin(CS43L22_AUDIO_RESET_GPIO_PORT, CS43L22_AUDIO_RESET_GPIO_PIN, GPIO_PIN_SET);
}

void CS43L22_write(uint8_t registry, uint8_t value){
	HAL_I2C_Mem_Write(&hi2cx, CS43L22_AUDIO_I2C_ADDR, registry, 1, &value, sizeof(value), HAL_MAX_DELAY);
}

uint8_t CS43L22_read(uint8_t registry){
	uint8_t value;
	HAL_I2C_Mem_Read(&hi2cx, CS43L22_AUDIO_I2C_ADDR, registry, 1, &value, sizeof(value), HAL_MAX_DELAY);
	return value;
}

void CS43L22_toggle_power(void){
	uint8_t current = CS43L22_read(CS43L22_REGISTRY_POWER_CONTROL_1);
	CS43L22_set_power(current ^ 0x01);
}

void CS43L22_set_power(CS43L22_POWER power){
    if(power==UP){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
    }
	CS43L22_write(CS43L22_REGISTRY_POWER_CONTROL_1, power);
}

void CS43L22_enable_channel(CS43L22_CHANNEL channel){
	CS43L22_write(CS43L22_REGISTRY_POWER_CONTROL_2, channel);
}

void CS43L22_start(uint16_t* audio_buffer, uint32_t len){
	HAL_I2S_Transmit_DMA(&hi2sx, (uint16_t*)audio_buffer, len / 2);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
}

void CS43L22_resume(void){
	HAL_I2S_DMAResume(&hi2sx);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
}

void CS43L22_pause(void){
	HAL_I2S_DMAPause(&hi2sx);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
}


void CS43L22_stop(void){
	HAL_I2S_DMAStop(&hi2sx);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
}

__weak void CS43L22_half_buffer_callback(void){

}

__weak void CS43L22_half_full_callback(void){

}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	if(hi2s->Instance == SPI3){
		CS43L22_half_buffer_callback();
	}
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	if(hi2s->Instance == SPI3){
		CS43L22_half_full_callback();
	}
}
