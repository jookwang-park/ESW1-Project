#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "farm_api.h"

void ULTRASONIC_trigger_on(int farm_fd) {
	ioctl(farm_fd, ULTRASONIC_ON, 0);
}

void ULTRASONIC_trigger_off(int farm_fd) {
	ioctl(farm_fd, ULTRASONIC_OFF, 0);
}

int ULTRASONIC_echo(int farm_fd) {
	int ret = 0;
	ioctl(farm_fd, ULTRASONIC_OFF, &ret);
	return ret;
}

int ULTRASONIC_distance(int farm_fd) {
	int distance;
	int value = 0;
	long start=0, end=0;
	ULTRASONIC_trigger_off(farm_fd);
	sleepm(300);
	ULTRASONIC_trigger_on(farm_fd);
	sleepu(10);
	ULTRASONIC_trigger_off(farm_fd);

	while( ULTRASONIC_echo(farm_fd) == 0 ) {
	}
	
	start = micros();
	while( ULTRASONIC_echo(farm_fd) == 1 ) {
	}
	end = start - micros();

	distance = end / 58;

	return distance;
}
