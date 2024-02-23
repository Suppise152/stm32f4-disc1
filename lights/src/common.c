#include "common.h"

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

static struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});
static struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios, {0});

/**
 * SETUP FUNCTIONS.
 */

// What it says on the tin.
void turnOffLEDs()
{
	TURN_OFF_GREEN_LED;
	TURN_OFF_BLUE_LED;
	TURN_OFF_RED_LED;
}

// What it says on the tin.
void turnOnLEDs()
{
	TURN_ON_GREEN_LED;
	TURN_ON_BLUE_LED;
	TURN_ON_RED_LED;
}

// Toggles LEDs with delay.
void flashLEDs(int delay)
{
	k_msleep(delay);
	TOGGLE_GREEN_LED;
	TOGGLE_BLUE_LED;
	TOGGLE_RED_LED;
	k_msleep(delay);
	TOGGLE_GREEN_LED;
	TOGGLE_BLUE_LED;
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
	TURN_ON_BLUE_LED;
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

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	// printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

// Sets up LEDs to output and button to input.
void SetUpDevice()
{

	printk("\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 35; j++) {
			printk("\n");
		}
		printk("Resetting\n");
		k_msleep(500);
		for (int j = 0; j < 35; j++) {
			printk("\n");
		}
		printk("Resetting.\n");
		k_msleep(500);
		for (int j = 0; j < 35; j++) {
			printk("\n");
		}
		printk("Resetting..\n");
		k_msleep(500);
		for (int j = 0; j < 35; j++) {
			printk("\n");
		}
		printk("Resetting...\n");
		k_msleep(500);
	}
	printk("\n");

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
	if (ledB.port && !gpio_is_ready_dt(&ledB)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n", ret, ledB.port->name);
		ledB.port = NULL;
	}
	if (ledR.port && !gpio_is_ready_dt(&ledR)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n", ret, ledR.port->name);
		ledR.port = NULL;
	}
	// Configure ports for output.
	if (ledG.port && ledB.port && ledR.port) {
		ret = gpio_pin_configure_dt(&ledG, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n", ret,
			       ledG.port->name, ledG.pin);
			ledG.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", ledG.port->name, ledG.pin);
		}
		ret = gpio_pin_configure_dt(&ledB, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n", ret,
			       ledB.port->name, ledB.pin);
			ledB.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", ledB.port->name, ledB.pin);
		}
		ret = gpio_pin_configure_dt(&ledR, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n", ret,
			       ledR.port->name, ledR.pin);
			ledR.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", ledR.port->name, ledR.pin);
		}
	}

	printk("\n");
	printk("Hello World\n");
	printk("\n");
}