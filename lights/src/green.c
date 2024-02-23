#include "green.h"

// Flashes green LED to indicate operation mode green has been selected.
void greenConfirm(void)
{
	printk("\n------------------------------\n");
	printk("Activating Green functionality");
	printk("\n------------------------------\n");
	for (int i = 0; i < 10; i++) {
		k_msleep(50);
		TURN_ON_GREEN_LED;
		k_msleep(50);
		TURN_OFF_GREEN_LED;
	}
}

void green(void)
{
	turnOffLEDs();
	greenConfirm();
	k_msleep(500);
	uint8_t count = 0;
	uint8_t decNum = count;
	uint8_t binNum = 0;
	uint8_t i = 1;

	uint8_t ones = 0;
	uint8_t tens = 0;
	uint8_t hundreds = 0;

	// 0 is unpressed, 1 is pressed.
	uint8_t buttonFlag = 0;
	while (1) {

		// Rising edge detector for button.
		if (buttonFlag == 0) {
			if (BUTTON_IS_PRESSED) {
				count++;
				if (count >= 8) {
					count = 0;
				}
				buttonFlag = 1;
				// Converts decimal count value into binary number for display.
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
				hundreds = binNum / 100 % 10;

				printk("\n");
				printk("-----------------\n");
				printk("\n");
				printk("Button pressed\n");
				printk("\n");
				printk("Count is %d\n", count);
				printk("Displaying %d %d %d\n", hundreds, tens, ones);
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
			TURN_ON_BLUE_LED
		} else {
			TURN_OFF_BLUE_LED;
		}
		if (hundreds) {
			TURN_ON_RED_LED
		} else {
			TURN_OFF_RED_LED;
		}
		k_msleep(SLEEP_TIME_MS);
	}
}
