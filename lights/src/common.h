// Zephyr includes.
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>

// General includes.
#include <inttypes.h>
#include <stdint.h>

#define SLEEP_TIME_MS 1

// LEDs.
#define TOGGLE_GREEN_LED gpio_pin_toggle_dt(&ledG);
#define TOGGLE_BLUE_LED gpio_pin_toggle_dt(&ledB);
#define TOGGLE_RED_LED gpio_pin_toggle_dt(&ledR);

#define TURN_ON_GREEN_LED gpio_pin_set_dt(&ledG, 1);
#define TURN_ON_BLUE_LED gpio_pin_set_dt(&ledB, 1);
#define TURN_ON_RED_LED gpio_pin_set_dt(&ledR, 1);

#define TURN_OFF_GREEN_LED gpio_pin_set_dt(&ledG, 0);
#define TURN_OFF_BLUE_LED gpio_pin_set_dt(&ledB, 0);
#define TURN_OFF_RED_LED gpio_pin_set_dt(&ledR, 0);

// Button.
#define BUTTON_IS_PRESSED gpio_pin_get_dt(&button)
#define SW0_NODE DT_ALIAS(sw0)

// Device admin.
void turnOffLEDs();
void turnOnLEDs();
void flashLEDs(int delay);
void startUp(int delay);
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void SetUpDevice();