#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "farm_api.h"

void led_on(int farm_fd) {
	ioctl(farm_fd, LED_ON, 0);
}

void led_off(int farm_fd) {
	ioctl(farm_fd, LED_OFF, 0);
}
