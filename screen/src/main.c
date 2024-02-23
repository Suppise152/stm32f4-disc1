/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

static uint32_t count;

#ifdef CONFIG_GPIO
static struct gpio_dt_spec button_gpio = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_callback;

static void button_isr_callback(const struct device *port, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	count = 0;
}
#endif /* CONFIG_GPIO */

#ifdef CONFIG_LV_Z_ENCODER_INPUT
static const struct device *lvgl_encoder =
	DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_encoder_input));
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

#define TOGGLE_GREEN_LED gpio_pin_toggle_dt(&ledG);
#define TOGGLE_RED_LED   gpio_pin_toggle_dt(&ledR);

#define TURN_ON_GREEN_LED gpio_pin_set_dt(&ledG, 1);
#define TURN_ON_RED_LED   gpio_pin_set_dt(&ledR, 1);

#define TURN_OFF_GREEN_LED gpio_pin_set_dt(&ledG, 0);
#define TURN_OFF_RED_LED   gpio_pin_set_dt(&ledR, 0);

static struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});

static int16_t red;
static int16_t green;
static int16_t blue;

static int16_t red2;
static int16_t green2;
static int16_t blue2;

static lv_obj_t * img1 = NULL;

static void lv_btn_click_callback(lv_event_t *e)
{
	ARG_UNUSED(e);
	count = 0;

	/* Create the image object if it doesn't exist */
    if(img1 == NULL) {
        img1 = lv_img_create(lv_scr_act());
        lv_img_set_src(img1, "download.jpg");
        lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    } else {
		lv_obj_del(img1);
		img1 = NULL;
	}
}

lv_color_t binCounter1(uint8_t count)
{
	uint8_t decNum = count;
	uint8_t binNum = 0;
	uint8_t i = 1;
	uint8_t ones = 0;
	uint8_t tens = 0;
	uint8_t hundreds = 0;

	// Converts decimal count value into binary number for display.
	while (count != 0) {
		binNum += (count % 2) * i;
		count /= 2;
		i *= 10;
	}

	ones = binNum % 10;
	tens = binNum / 10 % 10;
	hundreds = binNum / 100 % 10;

	printk("\n");
	printk("-----------------\n");
	printk("\n");
	printk("Count is %d\n", decNum);
	printk("Displaying %d %d %d\n", hundreds, tens, ones);

	// Incriments RGB value.
	if (ones) {
		blue++;
	} else {
		blue--;
	}
	if (tens) {
		green++;
	} else {
		green--;
	}
	if (hundreds) {
		red++;
	} else {
		red--;
	}

	// resets RGB value at max.
	if (red > 255) {
		red = 255;
	}
	if (green > 255) {
		green = 255;
	}
	if (blue > 255) {
		blue = 255;
	}

	// resets RGB value at min.
	if (red < 0) {
		red = 0;
	}
	if (green < 0) {
		green = 0;
	}
	if (blue < 0) {
		blue = 0;
	}
	return lv_color_make(red, green, blue);
}

lv_color_t binCounter2(uint8_t count)
{
	uint8_t decNum = count;
	uint8_t binNum = 0;
	uint8_t i = 1;
	uint8_t ones = 0;
	uint8_t tens = 0;
	uint8_t hundreds = 0;

	// Converts decimal count value into binary number for display.
	while (count != 0) {
		binNum += (count % 2) * i;
		count /= 2;
		i *= 10;
	}

	ones = binNum % 10;
	tens = binNum / 10 % 10;
	hundreds = binNum / 100 % 10;

	printk("\n");
	printk("-----------------\n");
	printk("\n");
	printk("Count is %d\n", decNum);
	printk("Displaying %d %d %d\n", hundreds, tens, ones);

	// Incriments RGB value.
	if (ones) {
		blue2++;
	} else {
		blue2--;
	}
	if (tens) {
		green2++;
	} else {
		green2--;
	}
	if (hundreds) {
		red2++;
	} else {
		red2--;
	}

	// resets RGB value at max.
	if (red2 > 255) {
		red2 = 255;
	}
	if (green2 > 255) {
		green2 = 255;
	}
	if (blue2 > 255) {
		blue2 = 255;
	}

	// resets RGB value at min.
	if (red2 < 0) {
		red2 = 0;
	}
	if (green2 < 0) {
		green2 = 0;
	}
	if (blue2 < 0) {
		blue2 = 0;
	}
	return lv_color_make(red2, green2, blue2);
}

int main(void)
{

	gpio_pin_configure_dt(&ledG, GPIO_OUTPUT);
	gpio_pin_configure_dt(&ledR, GPIO_OUTPUT);

	char count_str[11] = {0};
	char colour1_str[11] = {0};
    char colour2_str[11] = {0};
	char bin_str[11] = {0};
	const struct device *display_dev;
	lv_obj_t *hello_world_label;
	lv_obj_t *count_label;
	lv_obj_t *colour1_label;
    lv_obj_t *colour2_label;
	lv_obj_t *bin_label;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

#ifdef CONFIG_GPIO
	if (gpio_is_ready_dt(&button_gpio)) {
		int err;

		err = gpio_pin_configure_dt(&button_gpio, GPIO_INPUT);
		if (err) {
			LOG_ERR("failed to configure button gpio: %d", err);
			return 0;
		}

		gpio_init_callback(&button_callback, button_isr_callback, BIT(button_gpio.pin));

		err = gpio_add_callback(button_gpio.port, &button_callback);
		if (err) {
			LOG_ERR("failed to add button callback: %d", err);
			return 0;
		}

		err = gpio_pin_interrupt_configure_dt(&button_gpio, GPIO_INT_EDGE_TO_ACTIVE);
		if (err) {
			LOG_ERR("failed to enable button callback: %d", err);
			return 0;
		}
	}
#endif /* CONFIG_GPIO */

#ifdef CONFIG_LV_Z_ENCODER_INPUT
	lv_obj_t *arc;
	lv_group_t *arc_group;

	arc = lv_arc_create(lv_scr_act());
	lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(arc, 150, 150);

	arc_group = lv_group_create();
	lv_group_add_obj(arc_group, arc);
	lv_indev_set_group(lvgl_input_get_indev(lvgl_encoder), arc_group);
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

	if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN) || IS_ENABLED(CONFIG_LV_Z_POINTER_INPUT)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		lv_obj_add_event_cb(hello_world_button, lv_btn_click_callback, LV_EVENT_CLICKED,
				    NULL);
		hello_world_label = lv_label_create(hello_world_button);
	} else {
		hello_world_label = lv_label_create(lv_scr_act());
	}

	lv_label_set_text(hello_world_label, "Hello world!");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_scr_act());
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	colour1_label = lv_label_create(lv_scr_act());
	lv_obj_align(colour1_label, LV_ALIGN_TOP_LEFT, 0, 0);

    colour2_label = lv_label_create(lv_scr_act());
    lv_obj_align(colour2_label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

	bin_label = lv_label_create(lv_scr_act());
	lv_obj_align(bin_label, LV_ALIGN_TOP_MID, 0, 20);

	lv_task_handler();
	display_blanking_off(display_dev);
	display_blanking_off(DEVICE_DT_GET(DT_NODELABEL(ili9341)));

	TURN_ON_RED_LED;

	// Define a variable to hold the color value
	uint32_t colour1 = 0;
    uint32_t colour2 = 0;

	lv_obj_t *scr = lv_scr_act();

	// Create a style
	static lv_style_t style;
	lv_style_init(&style);

	// Apply the style to the screen
	lv_obj_add_style(scr, &style, 0);

	uint16_t update = 0;
	uint8_t cc1 = 1;
    uint8_t cc2 = 2;
	red = 0;
	green = 0;
	blue = 0;

	// Main loop
	while (1) {
		if ((count % 200) == 0U) {
			sprintf(count_str, "%d", count / 200U);
			lv_label_set_text(count_label, count_str);
		}
		TOGGLE_GREEN_LED;
		lv_task_handler();
		count++;

		// Set the style's background color to the current color value
		lv_style_set_bg_color(&style, binCounter1(cc1));
        lv_style_set_bg_grad_color(&style, binCounter2(cc2));
        lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);
		lv_obj_refresh_style(scr, LV_PART_MAIN, LV_STYLE_PROP_ALL);

		update++;
		if (update == 256) {
			cc1++;
            cc2++;
			update = 0;
		}
		if (cc1 == 8) {
			cc1 = 1;
		}
        if (cc2 == 8) {
            cc2 = 1;
        }

		colour1 = red << 16 | green << 8 | blue;
        colour2 = red2 << 16 | green2 << 8 | blue2;

		sprintf(bin_str, "%d", cc1);
		lv_label_set_text(bin_label, bin_str);

		sprintf(colour1_str, "%06X", colour1);
		lv_label_set_text(colour1_label, colour1_str);
        sprintf(colour2_str, "%06X", colour2);
		lv_label_set_text(colour2_label, colour2_str);

		k_msleep(5);
	}
}