/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.c"
#include "green.c"
#include "blue.c"
#include "red.c"

int main(void)
{
	SetUpDevice();

	turnOffLEDs();

	while (1) {

		TURN_ON_GREEN_LED;
		printk("\n");
		printk("Green\n");
		printk("\n");
		for (int i = 0; i < 200; i++) {
			if (BUTTON_IS_PRESSED) {
				green();
			}
			k_msleep(10);
		}
		TURN_OFF_GREEN_LED;
		k_msleep(750);

		TURN_ON_BLUE_LED;
		printk("\n");
		printk("Blue\n");
		printk("\n");
		for (int i = 0; i < 200; i++) {
			if (BUTTON_IS_PRESSED) {
				blue();
			}
			k_msleep(10);
		}
		TURN_OFF_BLUE_LED;
		k_msleep(750);

		TURN_ON_RED_LED;
		printk("\n");
		printk("Red\n");
		printk("\n");
		for (int i = 0; i < 200; i++) {
			if (BUTTON_IS_PRESSED) {
				red();
			}
			k_msleep(10);
		}
		TURN_OFF_RED_LED;
		k_msleep(750);
	}
	return 0;
}
