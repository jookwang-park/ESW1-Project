#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include "farm_api.h"
#include "spi_api.h"

static uint32_t speeds = 0;
static int fd = 0;

int SPI_DataRw(unsigned char *data, int len) {
	struct spi_ioc_transfer spi ;

	memset (&spi, 0, sizeof (spi)) ;

	spi.tx_buf        = (unsigned long)data ;
	spi.rx_buf        = (unsigned long)data ;
	spi.len           = len ;
	spi.delay_usecs   = SPI_DELAY;
	spi.speed_hz      = speeds;
	spi.bits_per_word = SPI_BPW;

	return ioctl(fd, SPI_IOC_MESSAGE(1), &spi);
}

int SPI_SetupMode(int speed, int mode) {
	mode &= 3;

	fd = open(SPI_DEV, O_RDWR);
	if( fd < 0 ) {
		perror("Open SPI Dev Error\n");
		return -1;
	}

	speeds = speed;

	if (ioctl (fd, SPI_IOC_WR_MODE, &mode) < 0) {
		perror("Set SPI Mode Error\n");
		return -1;
	}

	if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &SPI_BPW) < 0) {
		perror("Set SPI BPW Error\n");
		return -1;
	}

	if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
		perror("Set SPI Speed Error\n");
		return -1;
	}
	
	return fd ;
}

void SPI_On(int farm_fd) {
	ioctl(farm_fd, SPI_ON, 0);
}

void SPI_Off(int farm_fd) {
	ioctl(farm_fd, SPI_OFF, 0);
}

int SPI_Read_Value(int farm_fd, unsigned char channel) {
	unsigned char buff[3];
	int adcValue = 0;

	buff[0] = 0x06 | ((channel & 0x07) >> 7);
	buff[1] = ((channel & 0x07) << 6);
	buff[2] = 0x00;

	SPI_Off(farm_fd);

	SPI_DataRw(buff, 3);

	buff[1] = 0x0F & buff[1];
	adcValue = ( buff[1] << 8) | buff[2];

	SPI_On(farm_fd);

	return adcValue;
}

int SPI_Register_Handler(int farm_fd, unsigned char channel, int collect_time, int response_time, void (*handler)(int)) {
	int value = 0;
	int resCount = 0;
	int average = 0;
	int divider = response_time / collect_time;
	while(1) {
		value = SPI_Read_Value(farm_fd, channel);
		if( resCount == response_time ) {
			handler(average / divider);
			average = 0;
			resCount = 0;
		} else {
			resCount++;
			average += value;
		}
		sleep(collect_time);
	}
	return 0;
}

