# STM32-BMP388

This project is a firmware implementation for integrating the BMP388 high-precision barometric pressure sensor with an STM32F302R8 Nucleo board. The system acquires real-time pressure and temperature data to derive accurate altitude estimations via the barometric formula.


![20260106_172047_1 (2)](https://github.com/user-attachments/assets/ac80d752-c297-47d5-88b4-f80e70260a22)

---

### **``BMP388 - Barometric Pressure & Temperature Sensor``**

The BMP388 is a high-precision, low-power digital barometric pressure sensor. It measures both atmospheric pressure and ambient temperature, enabling accurate altitude tracking and environmental monitoring. It can be easily integrated with STM32-based systems via $I^2C$ and $SPI$ interfaces.

1. **``Pressure Sensor``**($Pa / hPa$)

   - Measures absolute atmospheric pressure.
   - Calculates altitude based on pressure changes (barometric altimeter principle).
   - High absolute accuracy and low long-term drift.
   - Adjustable output data rate (ODR) and filtering.
   - Operating range: 300 hPa to 1250 hPa.
  
3. **``Temperature Sensor``**($°C$)

   - Measures ambient temperature.
   - Primarily used for temperature compensation of the pressure sensor to ensure high accuracy.
  
---

### **``BMP388 Pins``**

![bmp388-digital-pressure-sensor-breakout-41745-11-O](https://github.com/user-attachments/assets/f07a884e-e08b-41e1-a982-a6cfe9923809)

The BMP388 digital barometric pressure sensor features a hybrid interface architecture that supports both **``I²C``** (Inter-Integrated Circuit) and **``SPI``** (Serial Peripheral Interface) communication protocols to provide flexible system integration.

1. #### `I²C Communication Mode`
   - **``VCC & GND:``** Power supply and ground reference lines for the sensor.
   - **``SCL (Serial Clock):``** Clock signal line.
   - **``SDA (Serial Data):``** Bidirectional data transmission line.
   - **``SDO:``** Used to define the I²C device address (Logic 0 or 1).

2. #### `SPI Communication Mode`
   - **``VCC & GND:``** Power supply and ground reference lines for the sensor.
   - **``CS (Chip Select):``** Used by the microcontroller to select the sensor and activate SPI mode.
   - **``SCK (Serial Clock):``** SPI clock signal line.
   - **``SDI (Serial Data In):``** Data input from the master device to the sensor (MOSI).
   - **``SDO (Serial Data Out):``** Data output from the sensor to the master device (MISO).

---

### **``STM32CubeIDE Configuration``**
We enable the I2C mode to activate the SDA and SCL pins.

<img width="1285" height="599" alt="image" src="https://github.com/user-attachments/assets/04bfaf90-dbd8-4670-b4b1-ffc86ac39298" />

- BMP388_VCC -> NucleoBoard_3.3V
- BMP388_GND -> NucleoBoard_GND
- BMP388_SCL ->  PA9 -> I2C2_SCL  
- BMP388_SDA -> PA10 -> I2C2_SDA
- BMP388_SDO -> NucleoBoard_GND

---

### 🛠️ **``How It Works?``**

The **``bmp388_interface_init``** function serves as the primary communication bridge between the Bosch Sensortec API and the STM32 hardware. Its key responsibilities include:
- **``Protocol Selection:``** Defines whether I2C or SPI will be used for physical data transmission.
- **``Address Mapping:``** Assigns the sensor's physical I2C address (default 0x76) to ensure correct data routing.
- **``Function Pointer Registration:``** Since the Bosch library is platform-independent, it does not have inherent knowledge of how to physically read or write data. Instead, it provides a flexible structure that holds the memory addresses (pointers) of the functions that will perform these tasks. By assigning the addresses of our custom-written **``bmp388_I2C_Read``**, **``bmp388_I2C_Write``**, and **``bmp388_delay_us``** functions to these pointers within the API, we establish the vital link between the hardware and the software.

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

```c
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

/* Wait 2ms to reset */
void bmp388_delay_us(uint32_t period, void *intf_ptr){

	HAL_Delay(period / 1000);
}

```

---

The **``BMP388_init``** function not only ensures that the sensor operates correctly but also defines how the sensor behaves according to the project requirements in terms of accuracy, sampling speed, and power consumption. This stage is the most critical part for ensuring data reliability and overall system stability. The main operations performed by this function are as follows:

-	**``Hardware and API Integration:``** The previously implemented communication interface (I2C) is activated, and the basic communication between the sensor and the Bosch API is initialized, including operations such as Chip ID verification.
-	**``Power Mode Selection (Power Control):``** The sensor is configured to operate in Normal Mode, enabling continuous measurements. Both the pressure and temperature sensing units are activated simultaneously.
-	**``Sampling and Noise Management (Oversampling & Filtering):``**
	-	**``Pressure Oversampling (8×):``** Since pressure measurements are highly sensitive to noise, eight samples are acquired per measurement cycle and internally averaged. This approach is essential for achieving high-precision altitude estimation.
	-	**``IIR Filtering:``** A low-pass Infinite Impulse Response (IIR) filter is applied to suppress sudden fluctuations in the measurement results, such as those caused by airflow due to device motion.
-	**``Output Data Rate (ODR – 50 Hz):``** The sensor is configured to generate new data at a rate of 50 samples per second, providing an optimal balance for real-time telemetry and altitude tracking applications.
-	**``Data Ready Interrupt:``** Instead of continuously polling the sensor to check for new data, the sensor is configured to interrupt the microcontroller (STM32) when data becomes available. This significantly improves CPU efficiency and power management.

```c
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
```

---

The **``BMP388_Read``** function acts as the “heartbeat” of the system; it acquires raw data from the sensor, validates it, and converts it into meaningful telemetry information such as pressure, temperature, and altitude. The operation of this function consists of the following stages:

-	**``Data Integrity Check (Status Check):``** Before reading data from the sensor, the Data Ready (DRDY) flag is checked using the bmp3_get_status function. This prevents the microcontroller from reading unprocessed or invalid data, thereby preserving data integrity.
-	**``Sensor Data Acquisition:``** Once the DRDY flag is confirmed, pressure and temperature values are read simultaneously via the Bosch API. After the read operation, the status register is checked again to clear the interrupt flags, ensuring that the sensor is ready for the next measurement cycle.
-	**``Unit Conversion:``** The raw pressure data obtained from the sensor is provided in Pascals (Pa). To comply with meteorological standards and the altitude calculation formula, this value is converted to hectopascals (hPa).

$$
1hPa=100Pa
$$

-	**``Barometric Altitude Formula:``** The most critical part of the function is the calculation of altitude above sea level using the pressure difference. The following International Standard Atmosphere (ISA) equation is applied:

$$
h = 44330 \cdot \left[ 1 - \left( \frac{P}{P_0} \right)^{0.1903} \right]
$$

**Where:**
1.	 **$P$**: The measured ambient atmospheric pressure (hPa).
2.	 **$P_0$**: The sea-level reference pressure (typically $1013.25$ hPa or the current local QNH setting).

-	**``Data Structuring:``** All computed values (pressure, temperature, and altitude) are stored in a dedicated data structure (DataStruct) to allow easy and efficient access by other parts of the project.

```c
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
```

---

-	**``BMP388_t Structure:``** It is used to collect and manage related data such as pressure, temperature, and altitude under a single structure. This approach enables organized and collective access to the sensor data.
-	**``BMP388_Read Function:``** It runs inside the main loop and updates the raw data received from the sensor in each iteration. This ensures continuous and real-time data acquisition, allowing the system to maintain an uninterrupted data flow.

```c
typedef struct {

	float Pressure;
	float Temperature;
	float Altitude;

} BMP388_t;

BMP388_t BMP388;
float temperature, pressure, baroAltitude;

while (1)
{
	BMP388_Read(&BMP388);
	pressure = BMP388.Pressure;
	temperature = BMP388.Temperature;
	baroAltitude = BMP388.Altitude;
	printf("Pressure: %.2f Temperature: %.2f BaroAltitude: %.2f \r\n", pressure, temperature, baroAltitude);
}

```



