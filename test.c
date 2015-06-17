#include <stdio.h>
#include <unistd.h>

int main(void) {
	int farm_fd = 0;
	farm_fd = farm_open();

	printf("Farm FD: %d\n", farm_fd);

	sleep(1);
	motor_on(farm_fd);
	sleep(3);
	motor_off(farm_fd);
	sleep(1);
	motor_on(farm_fd);
	sleep(3);
	motor_off(farm_fd);

	farm_close();
	return 0;
}
