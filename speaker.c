#include <linux/kernel.h>
#include <linux/gpio.h>

#include "farm.h"
#include "speaker.h"

MODULE_LICENSE("GPL");

void module_speaker_play(void) {
	gpio_set_value(SPEAKER_GPIO, 1);
}

void module_speaker_stop(void) {
	gpio_set_value(SPEAKER_GPIO, 0);
}

int module_speaker_init(void) {
	gpio_request_one(SPEAKER_GPIO, GPIOF_OUT_INIT_LOW, "speaker");
	return 0;
}

void module_speaker_exit(void) {
	gpio_set_value(SPEAKER_GPIO, 0);
	gpio_free(SPEAKER_GPIO);
}
