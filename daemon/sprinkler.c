#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "daemon.h"
#include "include/MQTTClient.h"
#include "include/MQTTAsync.h"

#define	SCHEDULE_SIZE	5

void sprinkler_scheduling();
void sprinkler_scheduled();
void sprinkler_running();
void sprinkler_start();
void sprinkler_end();
void sprinkler_state();

struct schedule_t {
	int id;
	int hour;
	int min;
	int is_run;
	int running_time;
};

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
static long running_time = 50; // deafult 5 sec
static struct schedule_t *schedules[SCHEDULE_SIZE];

int mqtt_onReceiver(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
	// 서버로부터 설정에 대한 값이 들어왔을 경우
	if( strncmp(topicName, topic_sprinkler_config, topicLen) == 0 ) {
		char **setting = (char**)get_setting((char*)message->payload, message->payloadlen);
		if( setting != NULL ) {
			if( strcmp(setting[0], "running_time") == 0 ) {
				printf("RUNNING_TIME: %s\n", setting[1]);
				running_time = atoi(setting[1]);
				destroy_setting(setting);
			}
		}
	}
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void add_schedule(int hour, int min) {
	int i = 0 ;
	for( i ; i < SCHEDULE_SIZE ; i++ ){
		if( schedules[i] == NULL ) {
			schedules[i] = (struct schedule_t*)malloc(sizeof(struct schedule_t));
			schedules[i]->id = i;
			schedules[i]->hour = hour;
			schedules[i]->min = min;
			schedules[i]->is_run = 0;
			schedules[i]->running_time = 30;
			return;
		}
	}
}

void edit_schedule(int id, int hour, int min, int running_time) {
	if( schedules[id] != NULL ) {
		schedules[id]->hour = hour;
		schedules[id]->min = min;
		schedules[id]->is_run = 0;
		schedules[id]->running_time = running_time;
	}
}

void delete_schedule(int id) {
	if( schedules[id] != NULL ) {
		free(schedules[id]);
		schedules[id] = NULL;
	}
}

void *sprinkler_init(void *data) {
	int rc = 0;
	pthread_t thread_sub;

	// Init
	info = *((struct daemon_info_t*)data);
	farm_fd = info.farm_fd;
	dev_name = info.dev_name;
	
	// Initialize MQTT Clients
	mqtt = (MQTTClient*)mqtt_create("sprinkler", mqtt_onReceiver);
	mqtt_topic(dev_name, "sprinkler/status", topic_sprinkler_status);
	mqtt_topic(dev_name, "sprinkler/config", topic_sprinkler_config);
	mqtt_pub(mqtt, "Init", topic_sprinkler_status, 0);
	mqtt_sub(mqtt, topic_sprinkler_config, 0);

	// Start Sprinkler Module
	state = SCHEDULING;
	while(1) {
		sprinkler_state();
		sleepu(100000); // 0.1 sec
	}
}

void sprinkler_scheduling() {
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	int i = 0;
	for(i = 0 ; i < SCHEDULE_SIZE ; i++ ){
		if( schedules[i] != NULL ) {
			int hour = schedules[i]->hour;
			int min = schedules[i]->min;
			int is_run = schedules[i]->is_run;
			if( t->tm_hour == hour && t->tm_min == min && is_run == 0 ) {
				state = SCHEDULED;
				schedules[i]->is_run = 1;
			} else if ( (t->tm_hour != hour || t->tm_min != min) && is_run == 1 ) {
				schedules[i]->is_run = 0;
			}
		}
	}
}

void sprinkler_scheduled() {
	mqtt_pub(mqtt, "Scheduled!", topic_sprinkler_status, 0);
	state =  START;
}

void sprinkler_start() {
	mqtt_pub(mqtt, "START!", topic_sprinkler_status, 0);
	mqtt_pub(mqtt, "RUNNING", topic_sprinkler_status, 0);
	state = RUNNING;
	motor_on(farm_fd);
}

void sprinkler_running() {
	if( t < running_time ) {
		t++;
	} else {
		t = 0;
		state = END;
	}
}

void sprinkler_end() {
	motor_off(farm_fd);
	mqtt_pub(mqtt, "END!", topic_sprinkler_status, 0);
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
