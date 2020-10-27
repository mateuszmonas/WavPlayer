#define CS43L22_AUDIO_I2C_ADDR 0x94
#define CS43L22_BUFFER_SIZE 2200
#define CS43L22_AUDIO_RESET_GPIO_PORT GPIOD
#define CS43L22_AUDIO_RESET_GPIO_PIN GPIO_PIN_4
#define CS43L22_REGISTRY_POWER_CONTROL_1 0x02
#define CS43L22_REGISTRY_POWER_CONTROL_2 0x04
#define CS43L22_REGISTRY_INTERFACE_CONRTOL_1 0x06

#include "stm32f4xx_hal.h"

typedef enum
{
	LEFT_HEADPHONE = 0xef,
	RIGHT_HEADPHONE = 0xbf,
	LEFT_SPEAKER = 0xfb,
	RIGHT_SPEAKER = 0xfe
}CS43L22_CHANNEL;

typedef enum
{
	UP = 0x9e,
	DOWN = 0x9f
}CS43L22_POWER;


void CS43L22_init(I2C_HandleTypeDef hi2c_handle, I2S_HandleTypeDef hi2s_handle);
void CS43L22_write(uint8_t registry, uint8_t value);
uint8_t CS43L22_read(uint8_t registry);
void CS43L22_play_tone(void);
void CS43L22_enable_channel(CS43L22_CHANNEL channel);
void CS43L22_set_power(CS43L22_POWER power);
void CS43L22_toggle_power(void);
