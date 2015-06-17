#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "farm_api.h"

void motor_on(int farm_fd) {
	ioctl(farm_fd, MOTOR_ON, 0);
}

void motor_off(int farm_fd) {
	ioctl(farm_fd, MOTOR_OFF, 0);
}
