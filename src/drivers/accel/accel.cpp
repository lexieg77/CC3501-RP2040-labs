#include "drivers/accel/accel.h"

#include <math.h>
#include <stdio.h>

#include <string>

#include "drivers/leds/leds.h"
#include "drivers/logging/logging.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "main.h"
#include "pico/stdlib.h"

// I2C Libraries:
#include <iostream>

#include "hardware/i2c.h"

void initialise_accel() {
  // Initialise the i2c bus at 400Hz
  i2c_init(I2C_INSTANCE, 400 * 1000);  // Telling the RP2040 to go into I2C mode
  gpio_set_function(Accel_SDA_MOSI, GPIO_FUNC_I2C);
  gpio_set_function(Aceel_SCL_SCLK, GPIO_FUNC_I2C);
  gpio_pull_up(Accel_SDA_MOSI);
  gpio_pull_up(Aceel_SCL_SCLK);
}

// Writing to the Accelerometer Register
void accel_write_register(uint8_t reg, uint8_t data) {
  uint8_t buf[2];
  buf[0] = reg;
  buf[1] = data;
  int bytes_written = i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, buf, 2,
                                         false);  // read 6 registers!!
  if (bytes_written != 2) {
    log(LogLevel::ERROR, "Failed to write to accelerometer register");
  }
}

// Reading registers
void accel_read_register(uint8_t reg, uint8_t *data_out) {
  if (1 != i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true)) {
    log(LogLevel::ERROR,
        "lis3dh::readregisters: Failed to select register address.");
  }

  int bytes_read =
      i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, data_out, 1, false);
  if (bytes_read != 1) {
    log(LogLevel::ERROR, "lis3dh::readregisters: Failed to read data.");
  }

  printf("data out: %f\n\r", data_out);
}

// Reading raw accelerometer data:
AccelVector accel_raw_data() {
  uint8_t reg = ACCEL_REG | 0x80;  // Set to multibyte read
  uint8_t raw_data[6];
  if (1 != i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true)) {
    log(LogLevel::ERROR,
        "lis3dh::readregisters: Failed to select register address.");
  }

  int bytes_read =
      i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, raw_data, 6, false);
  if (bytes_read != 6) {
    log(LogLevel::ERROR, "lis3dh::readregisters: Failed to read data.");
  }
  // Multibyte reading
  int16_t x_data = (int16_t)(raw_data[0] | (raw_data[1] << 8));
  int16_t y_data = (int16_t)(raw_data[2] | (raw_data[3] << 8));
  int16_t z_data = (int16_t)(raw_data[4] | (raw_data[5] << 8));

  // g's of each axis
  float x_accel = (float)x_data / G_CONVERSION;
  float y_accel = (float)y_data / G_CONVERSION;
  float z_accel = (float)z_data / G_CONVERSION;

  AccelVector vec;
  vec.x_accel = x_accel;
  vec.y_accel = y_accel;
  vec.z_accel = z_accel;
  printf("x gs: %f\r\n", x_accel);
  printf("y gs: %f\r\n", y_accel);
  printf("z gs: %f\r\n", z_accel);

  return vec;

  sleep_ms(1000);
}

void set_accel_leds(AccelVector vec, uint32_t led_data[12]) {
  if (abs(vec.x_accel) < 0.1) {
    for (int i = 0; i <= 5; i++) {
      led_colour_set(led_data, i, 0, 0, 255);  // green
    }
  } else if ((abs(vec.x_accel) > 0.1)) {
    for (int i = 0; i <= 3; i++) {
      led_colour_set(led_data, i, 255, 0, 0);  // red
    }
  }

  if (abs(vec.y_accel) < 0.1) {
    for (int i = 6; i <= 11; i++) {
      led_colour_set(led_data, i, 0, 0, 255);  // green
    }
  } else if ((abs(vec.y_accel) > 0.1)) {
    for (int i = 8; i <= 11; i++) {
      led_colour_set(led_data, i, 255, 0, 0);  // red
    }
  }
}
