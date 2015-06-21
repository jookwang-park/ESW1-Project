#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "farm_api.h"

void PIR_set_sensivity(int farm_fd, int sens) {
	ioctl(farm_fd, PIR_SET_SENS, &sens);
}

int PIR_is_detect(int farm_fd) {
	int ret = 0;
	ioctl(farm_fd, PIR_GET_DATA, &ret);
	return ret;
}
