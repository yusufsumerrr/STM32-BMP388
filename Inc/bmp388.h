
#ifndef INC_BMP388_H_
#define INC_BMP388_H_

#include "main.h"
#include "bmp3_defs.h"
#include "bmp3.h"
#include "I2C.h"

typedef struct {

	float Pressure;
	float Temperature;
	float Altitude;

} BMP388_t;

extern BMP388_t BMP388;


int8_t bmp388_interface_init(struct bmp3_dev *bmp3, uint8_t intf);
void BMP388_init();
void BMP388_Read(BMP388_t *DataStruct);
void bmp388_delay_us(uint32_t period, void *intf_ptr);

#endif /* INC_BMP388_H_ */
