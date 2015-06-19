#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "daemon.h"
#include "include/MQTTClient.h"

#define RUNNING_TIME	30 // 10 sec

void sprinkler_scheduling();
void sprinkler_scheduled();
void sprinkler_running();
void sprinkler_start();
void sprinkler_end();
void sprinkler_state();

enum SPRINKLER_STATE {
	INIT,
	SCHEDULING,
	SCHEDULED,
	START,
	RUNNING,
	END
};

static enum SPRINKLER_STATE state = INIT;

static long t = 0;
static long count = 0;
static char *dev_name = NULL;
static int farm_fd = 0;
static MQTTClient *mqtt = NULL;
static char topic_sprinkler_status[32] = {0,};
static char topic_sprinkler_config[32] = {0,};
static struct daemon_info_t info;

void sprinkler_status_receiver(int len, char *payload) {
	printf("%d %s\n", payload);
}

void *sprinkler_init(void *data) {
	int rc = 0;
	// Init
	info = *((struct daemon_info_t*)data);
	farm_fd = info.farm_fd;
	dev_name = info.dev_name;
	mqtt = info.mqtt;
	//
	mqtt_topic(dev_name, "sprinkler/status", topic_sprinkler_status);
	mqtt_topic(dev_name, "sprinkler/config", topic_sprinkler_config);
	mqtt_pub(dev_name, mqtt, "Init", topic_sprinkler_status, 0);
	rc = mqtt_sub(mqtt, topic_sprinkler_config, 2);
	mqtt_recv(mqtt, sprinkler_status_receiver);
	//
	state = SCHEDULING;
	while(1) {
		sprinkler_state();
		usleep(100000); // 0.1 sec
	}
}

void sprinkler_scheduling() {
	count++;
	if( count == 100 ) {
		count = 0;
		state = SCHEDULED;
	}
}

void sprinkler_scheduled() {
	mqtt_pub(dev_name, mqtt, "Scheduled!", topic_sprinkler_status, 0);
	state =  START;
}

void sprinkler_start() {
	mqtt_pub(dev_name, mqtt, "START!", topic_sprinkler_status, 0);
	mqtt_pub(dev_name, mqtt, "RUNNING", topic_sprinkler_status, 0);
	state = RUNNING;
	motor_on(farm_fd);
}

void sprinkler_running() {
	if( t < RUNNING_TIME ) {
		t++;
	} else {
		t = 0;
		state = END;
	}
}

void sprinkler_end() {
	motor_off(farm_fd);
	mqtt_pub(dev_name, mqtt, "END!", topic_sprinkler_status, 0);
	state = SCHEDULING;
}

void sprinkler_state() {
	switch(state) {
		case SCHEDULING:
			sprinkler_scheduling();
			break;
		case SCHEDULED:
			sprinkler_scheduled();
			break;
		case START:
			sprinkler_start();
			break;
		case RUNNING:
			sprinkler_running();
			break;
		case END:
			sprinkler_end();
			break;
		 default:
			break;
	}
}
