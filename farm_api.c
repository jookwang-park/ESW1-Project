#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "farm_api.h"

static int farm_fd = 0;

int farm_open() {
	farm_fd = open("/dev/"FARM_DEV, O_RDWR);
	return farm_fd;
}

void farm_close() {
	close(farm_fd);
}
