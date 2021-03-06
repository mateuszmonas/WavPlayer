#define LSM303_ACC_ADDRESS			(0x19 << 1)
#define LSM303_ACC_CTRL_REG1_A		0x20
#define LSM303_ACC_CTRL_REG3_A		0x22
#define LSM303_ACC_CTRL_REG5_A		0x24
#define LSM303_ACC_INT1_CFG_A		0x30
#define LSM303_ACC_INT1_SRC_A 		0x31
#define LSM303_ACC_INT1_THS_A 		0x32
#define LSM303_ACC_INT1_DURATION_A	0x33
#define LSM303_ACC_Y_L_A			0x2a
#define LSM303_ACC_Y_L_A_MULTI_READ (LSM303_ACC_Y_L_A | 0x80)
#define LSM303_ACC_RESOLUTION 2.0

#include "stm32f4xx_hal.h"

typedef enum{
	LSM303DLHC_INTERRUPT_STATE_ACTIVE=0,
	LSM303DLHC_INTERRUPT_STATE_INACTIVE
} LSM303DLHC_INTERRUPT_STATE;

typedef enum{
	LSM303DLHC_TILT_LEFT=0,
	LSM303DLHC_TILT_RIGHT
} LSM303DLHC_TILT;

void LSM303DLHC_init(I2C_HandleTypeDef hi2c1);
uint8_t LSM303DLHC_should_handle_interrupt(void);
LSM303DLHC_INTERRUPT_STATE LSM303DLHC_get_interrupt_state(void);
LSM303DLHC_TILT LSM303DLHC_get_tilt(void);
