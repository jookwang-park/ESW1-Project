#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include "farm_api.h"
#include "dht_api.h"

#define MAXTIMINGS	85

int dht11_dat[5] = { 0, 0, 0, 0, 0 };

struct dht_data DHT_get_data(int farm_fd)
{
	uint8_t laststate	= 1;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; /* fahrenheit */
	struct dht_data data;
 
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	/* pull pin down for 18 milliseconds */
	DHT_set_output(farm_fd);
	DHT_off(farm_fd);
	sleepm(18);
	/* then pull it up for 40 microseconds */
	DHT_on(farm_fd);
	sleepu( 40 );
	/* prepare to read the pin */
 	DHT_set_input(farm_fd);

	/* detect change and read data */
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( DHT_read_data(farm_fd) == laststate )
		{
			counter++;
			sleepu( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = DHT_read_data(farm_fd);

		if ( counter == 255 )
			break;
 
		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ) {
		data.humidity =  dht11_dat[0];
		data.temperature = dht11_dat[2];
	} else {
		data.humidity = 0;
		data.temperature = 0;
	}

	return data;
}

void DHT_set_input(int farm_fd) {
	ioctl(farm_fd, DHT_SET_INPUT, 0);
}

void DHT_set_output(int farm_fd) {
	ioctl(farm_fd, DHT_SET_OUTPUT, 0);
}

int DHT_read_data(int farm_fd) {
	int value = 0;
	ioctl(farm_fd, DHT_GET_DATA, &value);
	return value;
}

void DHT_on(int farm_fd) {
	ioctl(farm_fd, DHT_ON, 0);
}

void DHT_off(int farm_fd) {
	ioctl(farm_fd, DHT_OFF, 0);
}

