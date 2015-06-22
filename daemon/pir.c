#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "daemon.h"
#include "include/MQTTClient.h"

enum SPARROW_STATE {
	INIT,
	DETECTING,
	DETECTED
};

static char *dev_name = NULL;
static int farm_fd = 0;
static MQTTClient *mqtt = NULL;
static char topic_sparrow_status[32] = {0,};
static struct daemon_info_t info;
static enum SPARROW_STATE state = INIT;
static long wait_t = 0;

void sparrow_state();
void sparrow_detecting();
void sparrow_detected();

void sparrow_init(void *data) {
	int rc = 0;

	// Init
	info = *((struct daemon_info_t*)data);
	farm_fd = info.farm_fd;
	dev_name = info.dev_name;
	
	// Initialize MQTT Clients
	mqtt = (MQTTClient*)mqtt_create("sparrow", NULL);
	mqtt_topic(dev_name, "sparrow/status", topic_sparrow_status);

	//
	state = DETECTING;

	while(1) {
		sparrow_state();
		sleepu(100000); // 0.1 sec
	}
}

void sparrow_detecting() {
	int is_detect = 0;
	is_detect = PIR_is_detect(farm_fd);	
	if( is_detect == 1 ) {
		state = DETECTED;
		mqtt_pub(mqtt, "detect", topic_sparrow_status, 0);
		speaker_play(farm_fd, 1000, 1311);
	}
}

void sparrow_detected() {
	sleepm(2000);		
	speaker_stop(farm_fd);
	state = DETECTING;
}

void sparrow_state() {
	switch(state) {
		case DETECTING:
			sparrow_detecting();
			break;
		case DETECTED:
			sparrow_detected();
			break;
	}
}

