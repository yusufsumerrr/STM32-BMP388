
#include "bmp3.h"
#include "bmp388.h"
#include "bmp3_defs.h"
#include "I2C.h"
#include "stdio.h"
#include "math.h"
#include <stdint.h>
#include <stdio.h>

// Sea-level pressure (QNH) at LTAC / Ankara Esenboğa.
#define SEALEVELPRESSURE_HPA (1020)

static uint8_t dev_addr = 0;
int8_t rslt = 0;
uint16_t settings_sel;

struct bmp3_dev dev;
struct bmp3_data data = { 0 };
struct bmp3_settings settings = { 0 };
struct bmp3_status status = { { 0 } };


int8_t bmp388_interface_init(struct bmp3_dev *bmp3, uint8_t intf){

	int8_t rslt = BMP3_OK;    				// BMP3_OK = INT8_C(0)

	if(bmp3 != NULL){		 				// If the bmp3 structure is actually present in memory (if it is not empty)

		/* Bus configuration : I2C */
		if(intf == BMP3_I2C_INTF){			// BMP3_I2C_INTF = 1

			dev_addr = BMP3_ADDR_I2C_PRIM;  // if SDO pin = LOW, dev_addr = UINT8_C(0x76)
											// if SDO pin = HIGH, dev_addr = UINT8_C(0x77)
			bmp3->intf_ptr = &dev_addr;
			bmp3->intf = BMP3_I2C_INTF;
			bmp3->read = bmp388_I2C_Read;
			bmp3->write = bmp388_I2C_Write;
			bmp3->delay_us = bmp388_delay_us;
		}
	}
	else{rslt = BMP3_E_NULL_PTR;}

	return rslt;
}


void BMP388_init(){

    rslt = bmp388_interface_init(&dev, BMP3_I2C_INTF);
    rslt = bmp3_init(&dev);

    // Register 0x1B "PWR_CTRL"
    settings.op_mode = BMP3_MODE_NORMAL;
    settings.press_en = BMP3_ENABLE;
    settings.temp_en = BMP3_ENABLE;

    // Register 0x1C "OSR"
    settings.odr_filter.press_os = BMP3_OVERSAMPLING_8X;
    settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;

    // Register 0x1F "CONFIG"
    settings.odr_filter.iir_filter = BMP3_IIR_FILTER_COEFF_3;

    // Register 0x1D "ODR"
    settings.odr_filter.odr = BMP3_ODR_50_HZ;

    // Data Ready Interrupt
    settings.int_settings.drdy_en = BMP3_ENABLE;

    // Configure sensor settings: pressure, temperature, data ready flag, oversampling and ODR
    settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_DRDY_EN | BMP3_SEL_PRESS_OS |
    		BMP3_SEL_TEMP_OS | BMP3_SEL_ODR;

    rslt = bmp3_set_sensor_settings(settings_sel, &settings, &dev);
    rslt = bmp3_set_op_mode(&settings, &dev);

}


void BMP388_Read(BMP388_t *DataStruct){

	/* Retrieve device operational and interrupt status to verify data integrity */
	rslt = bmp3_get_status(&status, &dev);

	/* Execute data acquisition only if the Data Ready (DRDY) flag is asserted */
	if ((rslt == BMP3_OK) && (status.intr.drdy == BMP3_ENABLE)){

	    /*
	        * First parameter indicates the type of data to be read
	     	* BMP3_PRESS   : To read only pressure data
	     	* BMP3_TEMP    : To read only temperature data
	        * BMP3_PRESS_TEMP : To read pressure and temperature data
	    */
	    rslt = bmp3_get_sensor_data(BMP3_PRESS_TEMP, &data, &dev);

	    /* Re-access the status register to acknowledge and clear the pending interrupt service routine (ISR) flags.*/
	    rslt = bmp3_get_status(&status, &dev);

	    // Get pressure and temperature
	    float pressure = data.pressure;
	    float temperature = data.temperature;

	    float atmospheric = pressure / 100.0f;  // Pa to hPa

	    /*
	     	 * Calculate altitude
	     	 * h = 44330 × [1 - (P/P₀)^0.1903]
	     	 * P: The current ambient atmospheric pressure at your specific location.
	     	 * P_0 (SEALEVELPRESSURE_HPA): The reference pressure at mean sea level (MSL),
	     	 either as a standard value or the current local altimeter setting (QNH).
	    */
	    float altitude = 44330.0 * (1.0 - pow(atmospheric / SEALEVELPRESSURE_HPA, 0.1903));

	    DataStruct->Pressure = pressure;
	    DataStruct->Temperature = temperature;
	    DataStruct->Altitude = altitude;

	}
}


/* Wait 2ms to reset */
void bmp388_delay_us(uint32_t period, void *intf_ptr){

	HAL_Delay(period / 1000);

}



