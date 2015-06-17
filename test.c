#include <stdio.h>
#include <unistd.h>

int main(void) {
	int farm_fd = 0;
	farm_fd = farm_open();

	printf("Farm FD: %d\n", farm_fd);

	sleep(1);
//	motor_on(farm_fd);
	led_on(farm_fd);
	speaker_play(farm_fd, 500, 1911);
	sleep(3);
	motor_off(farm_fd);
	led_off(farm_fd);
	speaker_stop(farm_fd);
	sleep(1);
	led_on(farm_fd);
//	motor_on(farm_fd);
	speaker_play(farm_fd, 500, 1511);
	sleep(3);
	led_off(farm_fd);
	motor_off(farm_fd);
	speaker_stop(farm_fd);

	farm_close();
	return 0;
}
