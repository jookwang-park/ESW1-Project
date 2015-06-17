#include <linux/kernel.h>
#include <linux/gpio.h>

#include "farm.h"
#include "spi.h"

MODULE_LICENSE("GPL");

void module_spi_on(void) {
	gpio_set_value(SPI_GPIO, 1);
}

void module_spi_off(void) {
	gpio_set_value(SPI_GPIO, 0);
}

int module_spi_init(void) {
	gpio_request_one(SPI_GPIO, GPIOF_OUT_INIT_LOW, "spi");
	return 0;
}

void module_spi_exit(void) {
	gpio_set_value(SPI_GPIO, 0);
	gpio_free(SPI_GPIO);
}
