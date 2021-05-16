#define CS43L22_AUDIO_I2C_ADDR 0x94
#define CS43L22_AUDIO_RESET_GPIO_PORT GPIOD
#define CS43L22_AUDIO_RESET_GPIO_PIN GPIO_PIN_4
#define CS43L22_REGISTRY_POWER_CONTROL_1 0x02
#define CS43L22_REGISTRY_POWER_CONTROL_2 0x04
#define CS43L22_REGISTRY_INTERFACE_CONRTOL_1 0x06
#define CS43L22_REGISTRY_MASTER_VOLUME_CONRTOL_A 0x20
#define CS43L22_REGISTRY_MASTER_VOLUME_CONRTOL_B 0x21

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

typedef struct
{
  uint16_t   num_channels;
  uint32_t   sample_rate;
  uint16_t   bit_per_sample;
}CS43L22_DataFormatTypeDef;


void CS43L22_init(void);
uint8_t CS43L22_set_frequency(CS43L22_DataFormatTypeDef data_format);
void CS43L22_write(uint8_t registry, uint8_t value);
uint8_t CS43L22_read(uint8_t registry);
void CS43L22_enable_channel(CS43L22_CHANNEL channel);
void CS43L22_set_volume(uint8_t volume);
void CS43L22_set_power(CS43L22_POWER power);
void CS43L22_toggle_power(void);
void CS43L22_start(uint16_t* audio_buffer, uint32_t len, CS43L22_DataFormatTypeDef data_format);
void CS43L22_stop(void);
void CS43L22_half_buffer_callback(void);
void CS43L22_full_buffer_callback(void);
