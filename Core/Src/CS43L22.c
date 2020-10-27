#include "CS43L22.h"

#include "stm32f4xx_hal.h"
#include "math.h"

static I2C_HandleTypeDef hi2cx;
static I2S_HandleTypeDef hi2sx;
static int16_t audio_buffer[2 * CS43L22_BUFFER_SIZE];

void CS43L22_init(I2C_HandleTypeDef hi2c_handle, I2S_HandleTypeDef hi2s_handle){
	hi2cx = hi2c_handle;
	hi2sx = hi2s_handle;

	HAL_GPIO_WritePin(CS43L22_AUDIO_RESET_GPIO_PORT, CS43L22_AUDIO_RESET_GPIO_PIN, GPIO_PIN_SET);
	CS43L22_write(CS43L22_REGISTRY_INTERFACE_CONRTOL_1, 0x07);
	CS43L22_set_power(UP);

	CS43L22_enable_channel(LEFT_HEADPHONE & RIGHT_HEADPHONE);
}

void CS43L22_set_power(CS43L22_POWER power){
    if(power==UP){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
    }
	CS43L22_write(CS43L22_REGISTRY_POWER_CONTROL_1, power);
}

void CS43L22_toggle_power(void){
	uint8_t current = CS43L22_read(CS43L22_REGISTRY_POWER_CONTROL_1);
	CS43L22_set_power(current ^ 0x01);
//	CS43L22_write(CS43L22_REGISTRY_POWER_CONTROL_1, current ^ 0x01);
}

void CS43L22_enable_channel(CS43L22_CHANNEL channel){
	CS43L22_write(CS43L22_REGISTRY_POWER_CONTROL_2, channel);
}

void CS43L22_write(uint8_t registry, uint8_t value){
	HAL_I2C_Mem_Write(&hi2cx, CS43L22_AUDIO_I2C_ADDR, registry, 1, &value, sizeof(value), HAL_MAX_DELAY);
}

uint8_t CS43L22_read(uint8_t registry){
	uint8_t value;
	HAL_I2C_Mem_Read(&hi2cx, CS43L22_AUDIO_I2C_ADDR, registry, 1, &value, sizeof(value), HAL_MAX_DELAY);
	return value;
}

void fill_buffer(int start, int end){
	for (int i = start; i < end; i++) {
	        int16_t value = (int16_t)(32000.0 * sin(2.0 * M_PI * i / 22.0));
	        audio_buffer[i * 2] = value;
	        audio_buffer[i * 2 + 1] = value;
	}
}

void CS43L22_play_tone(void){
    HAL_I2S_Transmit_DMA(&hi2sx, (uint16_t*)audio_buffer, CS43L22_BUFFER_SIZE * 2);
}


void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
//	fill_buffer(0, CS43L22_BUFFER_SIZE/2);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
//	fill_buffer(CS43L22_BUFFER_SIZE/2, CS43L22_BUFFER_SIZE);
}
