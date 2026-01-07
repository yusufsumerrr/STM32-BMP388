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
- BMP388_SCL -> PA9  -> I2C2_SCL  
- BMP388_SDA -> PA10 -> I2C2_SDA 

---




