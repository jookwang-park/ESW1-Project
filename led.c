#include <linux/kernel.h>
#include <linux/gpio.h>

#include "farm.h"
#include "led.h"

MODULE_LICENSE("GPL");

void module_led_on(void) {
	gpio_set_value(LED_GPIO, 1);
}

void module_led_off(void) {
	gpio_set_value(LED_GPIO, 0);
}

int module_led_init(void) {
	gpio_request_one(LED_GPIO, GPIOF_OUT_INIT_LOW, "led1");
	return 0;
}

void module_led_exit(void) {
	gpio_set_value(LED_GPIO, 0);
	gpio_free(LED_GPIO);
}
