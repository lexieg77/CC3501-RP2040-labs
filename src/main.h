#pragma once

#define LED_PIN 14

#define I2C_INSTANCE i2c0
#define Accel_SDA_MOSI 16
#define Aceel_SCL_SCLK 17
#define I2C_ADDRESS  0x19 // Slave Address
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20 // 400Hz, low power mode and turning x, y and z axis on, 01111111
//#define CTRL_REG4 0x23 // Setting the scale to +-2g
#define ACCEL_REG 0x28 // Starting register
#define G_CONVERSION (32768/2) // 2^16/2 Conversion rate for +-2g
#define PI 3.14159