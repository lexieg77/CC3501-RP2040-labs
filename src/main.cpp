#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "main.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "drivers/accel/accel.h"

uint32_t led_data[12] = {0};
uint8_t data_out[6];

int main() {
    stdio_init_all();
    initialise_led();
    initialise_accel();
    log(LogLevel::INFORMATION, "Hello world"); // Test for log system
    accel_read_register(WHO_AM_I, data_out);
    accel_write_register(CTRL_REG1, 0x17);

    while(true){
        AccelVector vec = accel_raw_data();
        set_accel_leds(vec, led_data); 
        update_led(led_data);     
    }
    return 0;
} 
