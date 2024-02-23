#include "blue.h"

/**
 * FUNCTIONALITY FUNCTIONS.
 */

// Counts up to 7 in binary, displaying the number on the LEDs
// (1 is on, 0 is off).
void binCounter(int msDelay, uint8_t count)
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
	k_msleep(msDelay);
}

// Cycles through the LEDs, one at a time.
void cyclePartialFlash(int msDelay)
{
	k_msleep(msDelay);
	TURN_ON_GREEN_LED;
	k_msleep(msDelay);
	TURN_OFF_GREEN_LED;
	k_msleep(msDelay);
	TURN_ON_BLUE_LED;
	k_msleep(msDelay);
	TURN_OFF_BLUE_LED;
	k_msleep(msDelay);
	TURN_ON_RED_LED;
	k_msleep(msDelay);
	TURN_OFF_RED_LED;
	k_msleep(msDelay);
}

// Cycles through the LEDs all at a time.
void cycleFullFlash(int msDelay)
{
	k_msleep(msDelay);
	TURN_ON_GREEN_LED;
	k_msleep(msDelay);
	TURN_ON_BLUE_LED;
	k_msleep(msDelay);
	TURN_ON_RED_LED;
	k_msleep(msDelay);
	TURN_OFF_GREEN_LED;
	k_msleep(msDelay);
	TURN_OFF_BLUE_LED;
	k_msleep(msDelay);
	TURN_OFF_RED_LED;
	k_msleep(msDelay);
}

// Flashes all the LEDs on and off.
void fullFlash(int msDelay)
{
	k_msleep(msDelay);
	TURN_ON_GREEN_LED;
	TURN_ON_BLUE_LED;
	TURN_ON_RED_LED;
	k_msleep(msDelay);
	TURN_OFF_GREEN_LED;
	TURN_OFF_BLUE_LED;
	TURN_OFF_RED_LED;
}

// Cycles through the LEDs by turning one off at a time.
void voidCycle(int msDelay)
{
	k_msleep(msDelay);
	TURN_ON_GREEN_LED;
	TURN_ON_BLUE_LED;
	TURN_ON_RED_LED;
	k_msleep(msDelay);
	TURN_OFF_GREEN_LED;
	k_msleep(msDelay);
	TURN_ON_GREEN_LED;
	TURN_OFF_BLUE_LED;
	k_msleep(msDelay);
	TURN_ON_BLUE_LED;
	TURN_OFF_RED_LED;
}

// Flashes the green LED.
void greenFlash(int msDelay)
{
	k_msleep(msDelay);
	TURN_ON_GREEN_LED;
	TURN_OFF_BLUE_LED;
	TURN_OFF_RED_LED;
	k_msleep(msDelay);
	turnOffLEDs();
}

// Flashes the blue LED.
void blueFlash(int msDelay)
{
	k_msleep(msDelay);
	TURN_OFF_GREEN_LED;
	TURN_ON_BLUE_LED;
	TURN_OFF_RED_LED;
	k_msleep(msDelay);
	turnOffLEDs();
}

// Flashes the red LED.
void redFlash(int msDelay)
{
	k_msleep(msDelay);
	TURN_OFF_GREEN_LED;
	TURN_OFF_BLUE_LED;
	TURN_ON_RED_LED;
	k_msleep(msDelay);
	turnOffLEDs();
}

// Flashes blue LED to indicate operation mode blue has been selected.
void blueConfirm(void)
{
	printk("\n-----------------------------\n");
	printk("Activating Blue functionality");
	printk("\n-----------------------------\n");
	for (int i = 0; i < 10; i++) {
		k_msleep(50);
		TURN_ON_BLUE_LED;
		k_msleep(50);
		TURN_OFF_BLUE_LED;
	}
}

/**
 * MAIN FUNCTION.
 */

void blue(void)
{
	turnOffLEDs();
	blueConfirm();
	k_msleep(1000);

	// 0 is unpressed, 1 is pressed.
	uint8_t buttonFlag = 0;

	uint8_t display = 0;
	uint8_t count = 0;

	while (1) {

		// Rising edge detector for button.
		// When button is pushed, flash red LED as indicator, and change display to next.
		if (buttonFlag == 0) {
			if (BUTTON_IS_PRESSED) {
				display++;
				count = 0;
				buttonFlag = 1;
				TURN_OFF_GREEN_LED;
				TURN_OFF_BLUE_LED;
				TURN_ON_RED_LED;
				switch (display) {
				case 0:
					printk("\nBinary Counter Mode\n");
					break;
				case 1:
					printk("\nCycle Partial Flash Mode\n");
					break;
				case 2:
					printk("\nCycle Full Flash Mode\n");
					break;
				case 3:
					printk("\nFull Flash Mode\n");
					break;
				case 4:
					printk("\nVoid Cycle Mode\n");
					break;
				case 5:
					printk("\nGreen Flash Mode\n");
					break;
				case 6:
					printk("\nBlue Flash Mode\n");
					break;
				case 7:
					printk("\nRed Flash Mode\n");
					break;
				default:
					printk("\nReseting\n");
					break;
				}
				k_msleep(1250);
				TURN_OFF_RED_LED;
			}
		} else { // Rests flag.
			if (!BUTTON_IS_PRESSED) {
				buttonFlag = 0;
			}
		}

		// Different LED flash sequences.
		// Cycle through by pushing button.
		switch (display) {
		case 0:
			binCounter(500, count);
			count++;
			if (count >= 8) {
				count = 0;
			}
			break;
		case 1:
			cyclePartialFlash(50);
			break;
		case 2:
			cycleFullFlash(50);
			break;
		case 3:
			fullFlash(200);
			break;
		case 4:
			voidCycle(100);
			break;
		case 5:
			greenFlash(300);
			break;
		case 6:
			blueFlash(300);
			break;
		case 7:
			redFlash(300);
			break;
		default: // Reset display back to first.
			display = 0;
			break;
		}

		k_msleep(SLEEP_TIME_MS);
	}
}