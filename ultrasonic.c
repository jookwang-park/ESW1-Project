#include <linux/kernel.h>
#include <linux/gpio.h>

#include "farm.h"
#include "ultrasonic.h"

MODULE_LICENSE("GPL");

void module_ultrasonic_trigger_on(void) {
	gpio_set_value(ULTRASONIC_GPIO_OUT, 1);
}

void module_ultrasonic_trigger_off(void) {
	gpio_set_value(ULTRASONIC_GPIO_OUT, 0);
}

int module_ultrasonic_get_echo(void) {
	return gpio_get_value(ULTRASONIC_GPIO_IN);
}

int module_ultrasonic_init(void) {
	gpio_request_one(ULTRASONIC_GPIO_IN, GPIOF_IN, "ulstrasonic_in");
	gpio_request_one(ULTRASONIC_GPIO_OUT, GPIOF_OUT_INIT_LOW, "ultrasonic_out");
	return 0;
}

void module_ultrasonic_exit(void) {
	gpio_set_value(ULTRASONIC_GPIO_OUT, 0);
	gpio_free(ULTRASONIC_GPIO_OUT);
	gpio_free(ULTRASONIC_GPIO_IN);
}
