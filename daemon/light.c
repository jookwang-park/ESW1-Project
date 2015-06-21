#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "daemon.h"
#include "include/MQTTClient.h"
#include "../dht_api.h"


/*
State machine 
config->start->config   
init 에서 mqtt 설정하기
설정 받는 토픽 
dev2/config
SET light_x 10
SET light_y 10
SET light_z 800
*/
void light_handler(int avg);

void light_state();
void light_config();
void light_start();

int mqtt_light_onReceiver(void *context, char *topicName, int topicLen, MQTTClient_message *message);

enum LIGHT_STATE {
	INIT,
	CONFIG,
	START,
};
static enum LIGHT_STATE state = INIT;

enum SEASON {
	SPRING,//(3~5)
	SUMMER,//(6~8)
	FALL,//(9~11)
	WINTER//(12,1,2)
};
static enum SEASON season = SPRING;

enum NIGHTDAY {
	NIGHT,
	DAY
};
static enum NIGHTDAY nightday = NIGHT;

enum BRIGHT {
	DARKER,
	BRIGHTER,
	LITTLECHANE
};
static enum BRIGHT bright = LITTLECHANE;

enum WEATHER {	//  humidity, temperature
	NONE,		// 0 		, 0
	DRYNESS,	// 0~30		, 27~ 
	SUNNY,		// 30~50	, 27~ 
	CLOUDY,		// 50~70	, 23~
	RAINY  		// 70~		, 20~ 
};
static enum WEATHER weather = NONE;

static char *dev_name = NULL;
static int farm_fd = 0;
static struct daemon_info_t info;

static MQTTClient *mqtt = NULL;

static char topic_light_config[32] = {0, };

static int prevavglux = 0;
static int deltalux = 0;

static int light_x = 1;
static int light_y = 10;
static int light_z = 900;
static int humidity = 0;
static int temperature = 0;

int mqtt_light_onReceiver(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
	// 서버로부터 설정에 대한 값이 들어왔을 경우
	if( strncmp(topicName, topic_light_config, topicLen) == 0 ) {
		char **setting = (char**)get_setting((char*)message->payload, message->payloadlen);
		if( setting != NULL ) {

			if( strcmp(setting[0], "light_x") == 0 ) {
				printf("light_x: %s\n", setting[1]);
				light_x = atoi(setting[1]);
				destroy_setting(setting);
			}
			if( strcmp(setting[0], "light_y") == 0 ) {
				printf("light_y: %s\n", setting[1]);
				light_y = atoi(setting[1]);
				destroy_setting(setting);
			}
			if( strcmp(setting[0], "light_z") == 0 ) {
				printf("light_z: %s\n", setting[1]);
				light_z = atoi(setting[1]);
				destroy_setting(setting);
			}
		}
	}
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}


void *light_init(void *data) {

	info = *((struct daemon_info_t*)data);
	farm_fd = info.farm_fd;
	dev_name = info.dev_name;

	mqtt = (MQTTClient*)mqtt_create("light", mqtt_light_onReceiver);
	mqtt_topic(dev_name, "light/config", topic_light_config);

	mqtt_sub(mqtt, topic_light_config, 0);

	state = CONFIG;
	while(1) {
		light_state();
		usleep(100000);
	}
}




void light_config(){
	// 설정 변경.
	state=START;
}


void light_start(){
	SPI_Register_Handler(farm_fd, 0, &light_x, &light_y, light_handler);
	state=CONFIG;
}

void light_state() {
	switch(state) {
		case CONFIG:
		light_config();
		break;

		case START:
		light_start();
		break;

		default:
		break;
	}
}


void light_handler(int avg) {


	time_t timer;
	struct tm *t;

	humidity = ht_humidity();
	temperature = ht_temperature();

	timer = time(NULL); // 현재 시각을 초 단위로 얻기

	t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기
	t->tm_mon = t->tm_mon + 1;

	// Unix Time  timer // 1970-01-01 00:00:00 start.
	// year : t->tm_year + 1900);
	// mon : t->tm_mon + 1
	// t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec

	if(t->tm_mon>=3 && t->tm_mon<=5){
		season = SPRING;
		if(t->tm_hour <= 18 && t->tm_hour >= 5){
			nightday = 1;
		} else {
			nightday = 0;
		}
	} else if(t->tm_mon>=6 && t->tm_mon<=8){
		season = SUMMER;
		if(t->tm_hour <= 19 && t->tm_hour >= 5){
			nightday = 1;
		} else {
			nightday = 0;
		}
	} else if(t->tm_mon>=9 && t->tm_mon<=11){
		season = FALL;
		if(t->tm_hour <= 18 && t->tm_hour >= 5){
			nightday = 1;
		} else {
			nightday = 0;
		}
	}  else if(t->tm_mon>=12 || t->tm_mon<=2){
		season = WINTER;
		if(t->tm_hour <= 17 && t->tm_hour >= 6){
			nightday = 1;
		} else {
			nightday = 0;
		}
	}

	deltalux = prevavglux - avg;
	deltalux = -1 * deltalux;
	prevavglux = avg;

	//nightday==1  day
	if(nightday == 1){
		if(deltalux > light_z){
			bright = BRIGHTER;
			
			if(humidity<=30){
				if(temperature>27){
					weather=DRYNESS;
				}
			} else if(humidity>30 && humidity<=50 ){
				if(temperature>27){
					weather=SUNNY;
				}

			} else if(humidity<50 && humidity<=70) {
				if(temperature>23){
					weather=CLOUDY;
				}
			} else if(humidity>70){
				weather = RAINY;
			}
		} else if(deltalux < -1 * light_z) {
			bright=DARKER;

			if(humidity<=30){
				if(temperature>27){
					weather=DRYNESS;
				}
			} else if(humidity>30 && humidity<=50 ){
				if(temperature>27){
					weather=SUNNY;
				}

			} else if(humidity<50 && humidity<=70) {
				if(temperature>23){
					weather=CLOUDY;
				}
			} else if(humidity>70){
				weather = RAINY;
			}
		} else {
			bright=LITTLECHANE;
		}
	//nightday==0  night
	} else {
		weather=NONE;
	}
}

