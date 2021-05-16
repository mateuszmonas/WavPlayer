#include <LSM303DLHC.h>
#include <limits.h>

static I2C_HandleTypeDef hi2cx;

void LSM303DLHC_init(I2C_HandleTypeDef hi2c1){
	hi2cx = hi2c1;
	uint8_t ctrl_reg_1 = 0b10010010;
	uint8_t int_1_settings = 0b00001000;
	uint8_t int_1_treshold = 0b00001111;
	HAL_I2C_Mem_Write(&hi2cx, LSM303_ACC_ADDRESS, LSM303_ACC_CTRL_REG1_A, 1, &ctrl_reg_1, 1, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2cx, LSM303_ACC_ADDRESS, LSM303_ACC_INT1_CFG_A, 1, &int_1_settings, 1, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2cx, LSM303_ACC_ADDRESS, LSM303_ACC_INT1_THS_A, 1, &int_1_treshold, 1, HAL_MAX_DELAY);
}

uint8_t LSM303DLHC_should_handle_interrupt(void){
	static LSM303DLHC_INTERRUPT_STATE previous_interrupt_state = LSM303DLHC_INTERRUPT_STATE_INACTIVE;
	LSM303DLHC_INTERRUPT_STATE current_interrupt_state = LSM303DLHC_get_interrupt_state();
	uint8_t should_handle_interrupt = current_interrupt_state != previous_interrupt_state
									&& current_interrupt_state == LSM303DLHC_INTERRUPT_STATE_ACTIVE;
	previous_interrupt_state = current_interrupt_state;
	return should_handle_interrupt;
}

LSM303DLHC_INTERRUPT_STATE LSM303DLHC_get_interrupt_state(void){
	uint8_t int1_src_a;
	HAL_I2C_Mem_Read(&hi2cx, LSM303_ACC_ADDRESS, (LSM303_ACC_INT1_SRC_A), 1, &int1_src_a, sizeof(int1_src_a), HAL_MAX_DELAY);
	return int1_src_a & 0b01000000 ? LSM303DLHC_INTERRUPT_STATE_ACTIVE : LSM303DLHC_INTERRUPT_STATE_INACTIVE;
}

LSM303DLHC_TILT LSM303DLHC_get_tilt(void){
	uint8_t data[2];
	int16_t yaxis;
	HAL_I2C_Mem_Read(&hi2cx, LSM303_ACC_ADDRESS, (LSM303_ACC_Y_L_A_MULTI_READ), 1, data, 2, HAL_MAX_DELAY);
	yaxis = ((data[1] << 8) | data[0]);
	return 0 < yaxis ? LSM303DLHC_TILT_LEFT : LSM303DLHC_TILT_RIGHT;
}

