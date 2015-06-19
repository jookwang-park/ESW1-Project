#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include "farm.h"
#include "dht.h"

MODULE_LICENSE("GPL");

void module_dht_on(void) {
	gpio_set_value(DHT_GPIO, 1);
}

void module_dht_off(void) {
	gpio_set_value(DHT_GPIO, 0);
}

void module_dht_set_input(void) {
	int err = 0;
	err = gpio_direction_input(DHT_GPIO);
	if( err != 0 ){
		printk("Set GPIO Input Error\n");
	}
}

void module_dht_set_output(void) {
	int err = 0;
	err = gpio_direction_output(DHT_GPIO, 0);
	if( err != 0 ){
		printk("Set GPIO Output Error\n");
	}
}

int module_dht_get_value(void) {
	int value = gpio_get_value(DHT_GPIO);
	return value;
}

int module_dht_init(void) {
	int err = 0;
	err = gpio_request(DHT_GPIO, "DHT");
	return 0;
}

void module_dht_exit(void) {
	gpio_set_value(DHT_GPIO, 0);
	gpio_free(DHT_GPIO);
}
