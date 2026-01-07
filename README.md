# STM32-BMP388

This project is a firmware implementation for integrating the BMP388 high-precision barometric pressure sensor with an STM32 microcontroller. The system acquires real-time pressure and temperature data to derive accurate altitude estimations via the barometric formula.


![20260106_172047_1 (2)](https://github.com/user-attachments/assets/ac80d752-c297-47d5-88b4-f80e70260a22)

---

### `BMP388 - Barometric Pressure & Temperature Sensor`

The BMP388 is a high-precision, low-power digital barometric pressure sensor. It measures both atmospheric pressure and ambient temperature, enabling accurate altitude tracking and environmental monitoring. It can be easily integrated with STM32-based systems via $I^2C$ and $SPI$ interfaces.

1. `Pressure Sensor`($Pa / hPa$)

   - Measures absolute atmospheric pressure.
   - Calculates altitude based on pressure changes (barometric altimeter principle).
   - High absolute accuracy and low long-term drift.
   - Adjustable output data rate (ODR) and filtering.
   - Operating range: 300 hPa to 1250 hPa.
  
3. `Temperature Sensor`($°C$)

   - Measures ambient temperature.
   - Primarily used for temperature compensation of the pressure sensor to ensure high accuracy.
  
---

### `BMP388 Pins`

![bmp388-digital-pressure-sensor-breakout-41745-11-O](https://github.com/user-attachments/assets/f07a884e-e08b-41e1-a982-a6cfe9923809)

The BMP388 digital barometric pressure sensor features a hybrid interface architecture that supports both `I²C` (Inter-Integrated Circuit) and `SPI` (Serial Peripheral Interface) communication protocols to provide flexible system integration.

1. #### `I²C Communication Mode`
   - `VCC & GND:` Power supply and ground reference lines for the sensor.
   - `SCL (Serial Clock):` Clock signal line.
   - `SDA (Serial Data):` Bidirectional data transmission line.
   - `SDO:` Used to define the I²C device address (Logic 0 or 1).

2. #### `SPI Communication Mode`
   - `VCC & GND:` Power supply and ground reference lines for the sensor.
   - `CS (Chip Select):` Used by the microcontroller to select the sensor and activate SPI mode.
   - `SCK (Serial Clock):` SPI clock signal line.
   - `SDI (Serial Data In):` Data input from the master device to the sensor (MOSI).
   - `SDO (Serial Data Out):` Data output from the sensor to the master device (MISO).

---

### `STM32CubeIDE Configuration`
We enable the I2C mode to activate the SDA and SCL pins.

<img width="1285" height="599" alt="image" src="https://github.com/user-attachments/assets/04bfaf90-dbd8-4670-b4b1-ffc86ac39298" />

- BMP388_VCC -> NucleoBoard_3.3V
- BMP388_GND -> NucleoBoard_GND
- BMP388_SCL ->  PA9 -> I2C2_SCL  
- BMP388_SDA -> PA10 -> I2C2_SDA
- BMP388_SDO -> NucleoBoard_GND

---

### 🛠️ `How It Works?`

The bmp388_interface_init function serves as the primary communication bridge between the Bosch Sensortec API and the STM32 hardware. Its key responsibilities include:
- Protocol Selection: Defines whether I2C or SPI will be used for physical data transmission.
- Address Mapping: Assigns the sensor's physical I2C address (default 0x76) to ensure correct data routing.
- Function Pointer Registration: Since the Bosch library is platform-independent, it relies on pointers to execute hardware-specific tasks. We map these pointers to our custom STM32 routines:

  	1. bmp388_I2C_Read: Handles data acquisition.
	2. bmp388_I2C_Write: Manages register configuration.
	3. bmp388_delay_us: Provides precise timing for internal sensor operations.

```c
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
```

This function acts as a "handshake" or bridge between the Bosch API and the STM32 hardware. It begins with a safety check to ensure the sensor structure is properly allocated in memory. Once verified, it configures the communication protocol as I2C and assigns the hardware address (typically 0x76). The most critical part of this block is the function pointer mapping at the end. This tells the Bosch library to use specific STM32-based routines whenever it needs to perform a read, write, or delay operation. This approach maintains a modular design, allowing the high-level library to interact with the microcontroller without needing to handle low-level hardware details directly.



