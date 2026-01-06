
#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "main.h"
#include <stdint.h>


#define I2C_HANDLE	hi2c2
#define BUS_TIMEOUT 1000


int8_t bmp388_I2C_Read(uint8_t reg_addr, uint8_t *read_data, uint32_t len, void *intf_ptr);
int8_t bmp388_I2C_Write(uint8_t reg_addr, const uint8_t *read_data, uint32_t len, void *intf_ptr);
#endif /* INC_I2C_H_ */
