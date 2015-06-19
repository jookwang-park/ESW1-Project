#include <stdio.h>
#include <unistd.h>
#include "dht_api.h"

void gpio_test(int farm_fd) {
	sleep(1);
	motor_on(farm_fd);
	led_on(farm_fd);
	speaker_play(farm_fd, 500, 1911);
	sleep(3);
	motor_off(farm_fd);
	led_off(farm_fd);
	speaker_stop(farm_fd);
	sleep(1);
	led_on(farm_fd);
	motor_on(farm_fd);
	speaker_play(farm_fd, 500, 1511);
	sleep(3);
	led_off(farm_fd);
	motor_off(farm_fd);
	speaker_stop(farm_fd);
}

void light_handler(int avg) {
	printf("Light Value Average: %d\n", avg);
}

int main(void) {
	int farm_fd = 0;
	int adcValue = 0;
	int adcChannel = 1;
	farm_fd = farm_open();

	printf("Farm FD: %d\n", farm_fd);

	SPI_SetupMode(8000000, 0);
	//gpio_test(farm_fd);

	while(1) {
		struct dht_data dht;
		dht = DHT_get_data(farm_fd);
		if( dht.humidity == 0 && dht.temperature == 0 ) {
			continue;
		} else {
			printf("Humidity: %d%%, Temperature: %dC\n", dht.humidity, dht.temperature);
			sleep(1);
		}
	}
/*  
	while(1) {
		adcValue = SPI_Read_Value(farm_fd, 0);
		printf("adc0 Value = %u\n", adcValue);
	}
*/
//	SPI_Register_Handler(farm_fd, 0, 0, 1, light_handler);

	farm_close();
	return 0;
}
