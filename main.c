/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <mbed.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET(LED2_NODE, gpios);


int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&ledG)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&ledG, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&ledB, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&ledR, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		// ret = gpio_pin_toggle_dt(&ledG);
		// if (ret < 0) {
		// 	return 0;
		// }
		// k_msleep(SLEEP_TIME_MS);
		// ret = gpio_pin_toggle_dt(&ledB);
		// if (ret < 0) {
		// 	return 0;
		// }
		// k_msleep(SLEEP_TIME_MS);
		// ret = gpio_pin_toggle_dt(&ledR);
		// if (ret < 0) {
		// 	return 0;
		// }
		// k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
