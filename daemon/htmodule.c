#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "daemon.h"
#include "include/MQTTClient.h"
#include "../dht_api.h"


/*
H/T

pub : h, t 각각의 value
sub : 몇분주기로 체크 할것이냐.

센서 값 보내는 토픽
dev2/sensor/humidity
dev2/sensor/temperature


----완


측정 시간 받는 토픽
dev2/ht/config 
SET period <integer>






light에서

시간 +  밝기 + h + t
4가지 조건으로 

날씨 구분.

맑다
구름낌
비온다.
건조하다.

//sleepu(1000000); 1sec



펑션으로 가져오기.

*/





int ht_humidity();
int ht_temperature();
void htmodule_state();
void htmodule_config();
void htmodule_start();

int mqtt_htmodule_onReceiver(void *context, char *topicName, int topicLen, MQTTClient_message *message);




enum HT_STATE {
	INIT,
	CONFIG,
	START
};

static enum HT_STATE state = INIT;




static char *dev_name = NULL;
static int farm_fd = 0;
static struct daemon_info_t info;

static MQTTClient *mqtt = NULL;

static char topic_htmodule_config[32] = {0, };
static char topic_htmodule_humidity[32] = {0,};
static char topic_htmodule_temperature[32] = {0,};

static struct dht_data dht = {0,0};
static struct dht_data tempdht = {0,0};

static int ht_timetic = 1;// 1 == 1sec


int mqtt_htmodule_onReceiver(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
	// 서버로부터 설정에 대한 값이 들어왔을 경우
	if( strncmp(topicName, topic_htmodule_config, topicLen) == 0 ) {
		char **setting = (char**)get_setting((char*)message->payload, message->payloadlen);
		if( setting != NULL ) {

			if( strcmp(setting[0], "period") == 0 ) {
				printf("period: %s\n", setting[1]);
				ht_timetic = atoi(setting[1]);
				destroy_setting(setting);
			}
		}
		MQTTClient_freeMessage(&message);
		MQTTClient_free(topicName);
		return 1;
	}
}


void *htmodule_init(void *data) {
	info = *((struct daemon_info_t*)data);
	farm_fd = info.farm_fd;
	dev_name = info.dev_name;

	mqtt = (MQTTClient*)mqtt_create("ht", mqtt_htmodule_onReceiver);

	mqtt_topic(dev_name, "ht/config", topic_htmodule_config);
	mqtt_topic(dev_name, "sensor/temperature", topic_htmodule_temperature);
	mqtt_topic(dev_name, "sensor/humidity ", topic_htmodule_humidity);
	
	mqtt_sub(mqtt, topic_htmodule_config, 0);

	state = CONFIG;

	while(1) {
		htmodule_state();
		usleep(100000);
	}

}

void htmodule_config(){
	//ht 관련 셋팅.
	state=START;
}

void htmodule_start(){
	while(1){
		char buf[20] = {0,};
		sleep(ht_timetic);
		do{
			dht = DHT_get_data(farm_fd);
			if(dht.humidity != 0 && dht.temperature != 0 ){
				tempdht.humidity = dht.humidity;
				tempdht.temperature= dht.temperature;
			}
		}while(dht.humidity == 0 && dht.temperature == 0 );
		// sprintf(buf, "GET TEMPERATURE %d",dht.temperature);
		sprintf(buf, "%d",dht.temperature);
		mqtt_pub(mqtt, buf, topic_htmodule_temperature, 0);
		memset(buf,0, sizeof(buf));
		// sprintf(buf, "GET HUMIDITY %d", dht.humidity);
		sprintf(buf, "%d", dht.humidity);
		mqtt_pub(mqtt, buf, topic_htmodule_humidity, 0);
	}
}

void htmodule_state(){
	switch(state){
		case CONFIG:
		htmodule_config();
		break;

		case START:
		htmodule_start();
		break;

		default:
		break;
	}
}

int ht_humidity(){
	return tempdht.humidity;
};
int ht_temperature(){
	return tempdht.temperature;
}