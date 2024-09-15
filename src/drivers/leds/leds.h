#pragma once

// Setting the LED number and colour
void led_colour_set(uint32_t *led_data, int led_number, uint8_t red, uint8_t blue, uint8_t green);
void turn_all_leds_off(uint32_t *led_data, bool off);
void update_led(uint32_t *led_data);
void initialise_led();
void accel_level();