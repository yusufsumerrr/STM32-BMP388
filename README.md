# STM32-BMP388

This project is a firmware implementation for integrating the BMP388 high-precision barometric pressure sensor with an STM32 microcontroller. The system acquires real-time pressure and temperature data to derive accurate altitude estimations via the barometric formula.


![20260106_172047_1 (2)](https://github.com/user-attachments/assets/ac80d752-c297-47d5-88b4-f80e70260a22)

### `BMP388 - Barometric Pressure & Temperature Sensor`

The BMP388 is a high-precision, low-power digital barometric pressure sensor. It measures both atmospheric pressure and ambient temperature, enabling accurate altitude tracking and environmental monitoring. It can be easily integrated with STM32-based systems via $I^2C$ and $SPI$ interfaces.

1. `Pressure Sensor`(Pa / hPa)
   - Measures absolute atmospheric pressure.
   - 
Calculates altitude based on pressure changes (barometric altimeter principle).
High absolute accuracy and low long-term drift.
Adjustable output data rate (ODR) and filtering.
Operating range: 300 hPa to 1250 hPa.

