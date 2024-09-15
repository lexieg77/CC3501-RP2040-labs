#pragma once
#include <cstdint>
struct AccelVector {
    float x_accel;
    float y_accel;
    float z_accel;   
};

void initialise_accel();
void accel_write_register(uint8_t reg, uint8_t data);
void accel_read_register(uint8_t reg, uint8_t* data_out);
AccelVector accel_raw_data();
void set_accel_leds(AccelVector vec, uint32_t led_data[12]);