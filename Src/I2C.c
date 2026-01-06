
#include "main.h"
#include "I2C.h"
#include "bmp388.h"



extern I2C_HandleTypeDef hi2c2;


int8_t bmp388_I2C_Read(uint8_t reg_addr, uint8_t *read_data, uint32_t len, void *intf_ptr){

	uint8_t dev_addr = *(uint8_t *)intf_ptr;
	if(HAL_I2C_Mem_Read(&I2C_HANDLE, (dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, read_data, len, BUS_TIMEOUT) == HAL_OK){
		return 0;   // BMP3_OK            INT8_C(0)
	} else{
		return -2;  // BMP3_E_COMM_FAIL   INT8_C(-2)
	}
}


int8_t bmp388_I2C_Write(uint8_t reg_addr, const uint8_t *read_data, uint32_t len, void *intf_ptr){

	uint8_t dev_addr = *(uint8_t *)intf_ptr;
	if(HAL_I2C_Mem_Write(&I2C_HANDLE, (dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)read_data, len, BUS_TIMEOUT) == HAL_OK){
		return 0;   // BMP3_OK            INT8_C(0)
	} else{
		return -2;  // BMP3_E_COMM_FAIL   INT8_C(-2)
	}
}


















