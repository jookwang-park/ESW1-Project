#include <linux/kernel.h>
#include <linux/gpio.h>

#include "farm.h"
#include "motor.h"

MODULE_LICENSE("GPL");

void module_motor_on(void) {
	gpio_set_value(MOTOR_GPIO, 1);
}

void module_motor_off(void) {
	gpio_set_value(MOTOR_GPIO, 0);
}

int module_motor_init(void) {
	gpio_request_one(MOTOR_GPIO, GPIOF_OUT_INIT_LOW, "motor");
	return 0;
}

void module_motor_exit(void) {
	gpio_set_value(MOTOR_GPIO, 0);
	gpio_free(MOTOR_GPIO);
}
