#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "WS2812.pio.h"
#include "main.h"

void led_colour_set(uint32_t *led_data, int led_number, uint8_t red, uint8_t blue, uint8_t green){
        led_data[led_number] = (red << 24) | (green << 16) | (blue << 8);       
}

void update_led(uint32_t *led_data){
        // For loop so the driver knows where each LED is
        for (int i = 0; i <= 11; i++) {
                pio_sm_put_blocking(pio0, 0, led_data[i]);
        }
}

void turn_all_leds_off(uint32_t *led_data, bool off){
        if (off == true){
                for (int i = 0; i <= 11; i++) {
                        led_data[i] = 0;
                        pio_sm_put_blocking(pio0, 0, led_data[i]);
                }
        }      
}

void initialise_led() {
    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
}