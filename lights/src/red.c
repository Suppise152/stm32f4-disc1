#include "red.h"

// Cycles through the LEDs, one at a time.
void cycleFlashGame(int msDelay)
{
	TURN_ON_GREEN_LED;
	k_msleep(1);
	for (int i = 0; i < 10; i++) {
		if (BUTTON_IS_PRESSED) {
			turnOffLEDs();
			printk("Missed\n");
			redConfirm();
			return;
		}
		k_msleep(10);
	}
	TURN_OFF_GREEN_LED;
	k_msleep(msDelay);
	TURN_ON_BLUE_LED;
	k_msleep(1);
	for (int i = 0; i < 10; i++) {
		if (BUTTON_IS_PRESSED) {
			turnOffLEDs();
			printk("Hit\n");
			greenConfirmGame();
			return;
		}
		k_msleep(10);
	}
	TURN_OFF_BLUE_LED;
	k_msleep(msDelay);
	TURN_ON_RED_LED;
	k_msleep(1);
	for (int i = 0; i < 10; i++) {
		if (BUTTON_IS_PRESSED) {
			turnOffLEDs();
			printk("Missed\n");
			redConfirm();
			return;
		}
		k_msleep(10);
	}
	TURN_OFF_RED_LED;
	k_msleep(msDelay);
}

void greenConfirmGame(void)
{
	for (int i = 0; i < 10; i++) {
		k_msleep(50);
		TURN_ON_GREEN_LED;
		k_msleep(50);
		TURN_OFF_GREEN_LED;
	}
	k_msleep(1000);
}

// Flashes red LED to indicate operation mode red has been selected.
void redConfirm(void)
{
	for (int i = 0; i < 10; i++) {
		k_msleep(50);
		TURN_ON_RED_LED;
		k_msleep(50);
		TURN_OFF_RED_LED;
	}
	k_msleep(1000);
}

void red(void)
{
	printk("\n----------------------------\n");
	printk("Activating Red functionality");
	printk("\n----------------------------\n");
	turnOffLEDs();
	redConfirm();
	while (1) {
		cycleFlashGame(50);
	}
}