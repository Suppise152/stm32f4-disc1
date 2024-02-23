/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <stdint.h>

#define SLEEP_TIME_MS 1

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

#define TOGGLE_GREEN_LED gpio_pin_toggle_dt(&ledG);
// #define TOGGLE_BLUE_LED gpio_pin_toggle_dt(&ledB);
#define TOGGLE_RED_LED   gpio_pin_toggle_dt(&ledR);

#define TURN_ON_GREEN_LED gpio_pin_set_dt(&ledG, 1);
// #define TURN_ON_BLUE_LED gpio_pin_set_dt(&ledB, 1);
#define TURN_ON_RED_LED   gpio_pin_set_dt(&ledR, 1);

#define TURN_OFF_GREEN_LED gpio_pin_set_dt(&ledG, 0);
// #define TURN_OFF_BLUE_LED gpio_pin_set_dt(&ledB, 0);
#define TURN_OFF_RED_LED   gpio_pin_set_dt(&ledR, 0);

#define BUTTON_IS_PRESSED gpio_pin_get_dt(&button)

/*
 * The led0 devicetree alias is optional. If present, we'll use it
 * to turn on the LED whenever the button is pressed.
 */
static struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
// static struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});
static struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	// printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

// What it says on the tin.
void turnOffLEDs()
{
	TURN_OFF_GREEN_LED;
	// TURN_OFF_BLUE_LED;
	TURN_OFF_RED_LED;
}

// Toggles LEDs with delay.
void flashLEDs(int delay)
{
	k_msleep(delay);
	TOGGLE_GREEN_LED;
	// TOGGLE_BLUE_LED;
	TOGGLE_RED_LED;
	k_msleep(delay);
	TOGGLE_GREEN_LED;
	// TOGGLE_BLUE_LED;
	TOGGLE_RED_LED;
	k_msleep(delay);
}

void SetUpDevice()
{

	int ret;

	// Set up push button.
	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n", button.port->name);
		return;
	}
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n", ret, button.port->name,
		       button.pin);
		return;
	}
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n", ret,
		       button.port->name, button.pin);
		return;
	}
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	// Configure LEDs
	if (ledG.port && !gpio_is_ready_dt(&ledG)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n", ret, ledG.port->name);
		ledG.port = NULL;
	}
	// if (ledB.port && !gpio_is_ready_dt(&ledB)) {
	// 	printk("Error %d: LED device %s is not ready; ignoring it\n", ret, ledB.port->name);
	// 	ledB.port = NULL;
	// }
	if (ledR.port && !gpio_is_ready_dt(&ledR)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n", ret, ledR.port->name);
		ledR.port = NULL;
	}
	// Configure ports for output.
	if (ledG.port && ledR.port) {
		ret = gpio_pin_configure_dt(&ledG, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n", ret,
			       ledG.port->name, ledG.pin);
			ledG.port = NULL;
		} else {
			printk("Set up green LED at %s pin %d\n", ledG.port->name, ledG.pin);
		}
		// ret = gpio_pin_configure_dt(&ledB, GPIO_OUTPUT);
		// if (ret != 0) {
		// 	printk("Error %d: failed to configure LED device %s pin %d\n", ret,
		// 	       ledB.port->name, ledB.pin);
		// 	ledB.port = NULL;
		// } else {
		// 	printk("Set up LED at %s pin %d\n", ledB.port->name, ledB.pin);
		// }
		ret = gpio_pin_configure_dt(&ledR, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n", ret,
			       ledR.port->name, ledR.pin);
			ledR.port = NULL;
		} else {
			printk("Set up red LED at %s pin %d\n", ledR.port->name, ledR.pin);
		}
	}

	printk("\n");
	printk("Hello World\n");
	printk("\n");
}

int main(void)
{
	SetUpDevice();

	turnOffLEDs();
	uint8_t count = 0;
	uint8_t decNum = count;
	uint8_t binNum = 0;
	uint8_t i = 1;
	uint8_t ones = 0;
	uint8_t tens = 0;

	k_msleep(200);

	// 0 is unpressed, 1 is pressed.
	uint8_t buttonFlag = 0;
	if (ledG.port) {
		while (1) {

			// Rising edge detector for button.
			if (buttonFlag == 0) {
				if (BUTTON_IS_PRESSED) {
					count++;
					if (count >= 4) {
						count = 0;
					}
					buttonFlag = 1;

					// Converts decimal count value into binary number for
					// display.
					decNum = count;
					binNum = 0;
					i = 1;
					while (decNum != 0) {
						binNum += (decNum % 2) * i;
						decNum /= 2;
						i *= 10;
					}
					ones = binNum % 10;
					tens = binNum / 10 % 10;

					printk("\n");
					printk("-----------------\n");
					printk("\n");
					printk("Button pressed\n");
					printk("\n");
					printk("Count is %d\n", count);
					printk("Displaying %d %d\n", tens, ones);
				}
			} else {
				if (!BUTTON_IS_PRESSED) {
					buttonFlag = 0;
				}
			}

			// Displayes count on the board.
			if (ones) {
				TURN_ON_GREEN_LED;
			} else {
				TURN_OFF_GREEN_LED;
			}
			if (tens) {
				TURN_ON_RED_LED
			} else {
				TURN_OFF_RED_LED;
			}
			k_msleep(SLEEP_TIME_MS);
		}
	}
	return 0;
}
