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

#include <zephyr/drivers/display.h>
#include <C:/Users/tim.cashmore/zephyrproject/lvgl/lvgl.h>
#include <stdio.h>
#include <string.h>

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
#define TOGGLE_RED_LED   gpio_pin_toggle_dt(&ledR);

#define TURN_ON_GREEN_LED gpio_pin_set_dt(&ledG, 1);
#define TURN_ON_RED_LED   gpio_pin_set_dt(&ledR, 1);

#define TURN_OFF_GREEN_LED gpio_pin_set_dt(&ledG, 0);
#define TURN_OFF_RED_LED   gpio_pin_set_dt(&ledR, 0);

#define BUTTON_IS_PRESSED gpio_pin_get_dt(&button)

#define BUF_W 20
#define BUF_H 10

const struct device *lvgl_encoder = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(&stmpe811));

/*
 * Initiates LEDs.
 */
static struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	// printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

// What it says on the tin.
void turnOffLEDs()
{
	TURN_OFF_GREEN_LED;
	TURN_OFF_RED_LED;
}

// Toggles LEDs with delay.
void flashLEDs(int delay)
{
	k_msleep(delay);
	TOGGLE_GREEN_LED;
	TOGGLE_RED_LED;
	k_msleep(delay);
	TOGGLE_GREEN_LED;
	TOGGLE_RED_LED;
	k_msleep(delay);
}

// Start up sequence to indicate button has been pushed.
void startUp(int delay)
{
	turnOffLEDs();
	k_msleep(delay);
	TURN_ON_GREEN_LED;
	k_msleep(delay);
	TURN_ON_RED_LED;
	k_msleep(delay);
	turnOffLEDs();
	flashLEDs(100);
	flashLEDs(100);
	flashLEDs(100);
	flashLEDs(100);
	flashLEDs(100);
	turnOffLEDs();
}

int main(void)
{
	lv_init();
	


	lv_obj_t *arc;
	lv_group_t *arc_group;

	arc = lv_arc_create(lv_screen_active());
	lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(arc, 150, 150);

	arc_group = lv_group_create();
	lv_group_add_obj(arc_group, arc);
	lv_indev_set_group(lvgl_input_get_indev(lvgl_encoder), arc_group);

	lv_obj_t *label = lv_label_create(lv_scr_act());
	lv_label_set_text(label, "Hello world");
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	// lv_init();
	// /*Change the active screen's background color*/
	// lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

	// /*Create a white label, set its text and align it to the center*/
	// lv_obj_t *label = lv_label_create(lv_scr_act());
	// lv_label_set_text(label, "Hello world");
	// lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);
	// lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

	int ret;

	{
		if (!gpio_is_ready_dt(&button)) {
			printk("Error: button device %s is not ready\n", button.port->name);
			return 0;
		}

		ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure %s pin %d\n", ret, button.port->name,
			       button.pin);
			return 0;
		}

		ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt on %s pin %d\n", ret,
			       button.port->name, button.pin);
			return 0;
		}

		gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
		gpio_add_callback(button.port, &button_cb_data);
		printk("Set up button at %s pin %d\n", button.port->name, button.pin);

		if (ledG.port && !gpio_is_ready_dt(&ledG)) {
			printk("Error %d: LED device %s is not ready; ignoring it\n", ret,
			       ledG.port->name);
			ledG.port = NULL;
		}
		if (ledG.port) {
			ret = gpio_pin_configure_dt(&ledG, GPIO_OUTPUT);
			ret = gpio_pin_configure_dt(&ledR, GPIO_OUTPUT);
			if (ret != 0) {
				printk("Error %d: failed to configure LED device %s pin %d\n", ret,
				       ledG.port->name, ledG.pin);
				ledG.port = NULL;
			} else {
				printk("Set up LED at %s pin %d\n", ledG.port->name, ledG.pin);
			}
		}
	}

	printk("Press the button\n");
	while (1) {

		// Red LED on by default.
		// Shows that button is not pushed.
		TURN_ON_RED_LED;

		// Run LED flashing sequence if button is pushed.
		int val = BUTTON_IS_PRESSED;

		if (val > 0) {
			startUp(1000);
			// Cycle through LEDs 10 times.
			// If button is pushed, end sequence.
			for (uint8_t i = 0; i < 10; i++) {
				TURN_ON_GREEN_LED;
				k_msleep(500);
				TURN_OFF_GREEN_LED;
				k_msleep(500);
				if (gpio_pin_get_dt(&button)) {
					break;
				}
				TURN_ON_RED_LED;
				k_msleep(500);
				TURN_OFF_RED_LED;
				k_msleep(500);
				if (gpio_pin_get_dt(&button)) {
					break;
				}
			}

			// End of sequence.
			flashLEDs(200);
			flashLEDs(200);
			flashLEDs(200);
		}
		k_msleep(SLEEP_TIME_MS);
		TURN_ON_RED_LED;
	}
	return 0;
}
