#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/MQTTClient.h"
#include "include/MQTTAsync.h"

#define	MQTT_ADDRESS		"tcp://brainguys.net:1883"
#define	MQTT_TIMEOUT		10000L

/* *
 * 시스템 파일에 로그를 남겨준다
 * /tmp/farm/filename 에 남는다 
 * */
void logging(char *filename, char str[255]) {
	FILE *log = NULL;
	char path[255] = {0,};
	char log_str[255] = {0,};
	sprintf(path, "/tmp/farm/%s", filename);
	mkdir("/tmp/farm");
	log = fopen(path, "a");
	sprintf(log_str, "%s\n", str);
	fprintf(log, log_str);
	fclose(log);
}

/* *
 * MQTT Topic
 * MQTT에서 사용할 토픽을 보다 쉽게 만들어주기 위한 함수 
 * */
void mqtt_topic(char *dev_name, char *topic, char *dest) {
	char buf[255] = {0,};
	sprintf(buf, "%s/%s", dev_name, topic);
	strncpy(dest, buf, strlen(buf));
}

MQTTClient *mqtt_create(char *clientid, MQTTClient_messageArrived *arrived) {
	int rc = 0;
	MQTTClient *context = (MQTTClient*)malloc(sizeof(MQTTClient));
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	MQTTClient_create(context, MQTT_ADDRESS, clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 0;
    conn_opts.cleansession = 1;
	if(arrived) MQTTClient_setCallbacks(*context, NULL, NULL, arrived, NULL);
	MQTTClient_connect(*context, &conn_opts);
	return context;
}

int mqtt_pub(MQTTClient *context, char *payload, char *topic, int qos) {
	int rc = 0;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	pubmsg.payload = payload;
	pubmsg.payloadlen = strlen(payload);
	pubmsg.qos = qos;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(*context, topic, &pubmsg, &token);
	return rc;
}

int mqtt_sub(MQTTClient *context, char *topic, int qos) {
	int rc = 0;
	rc = MQTTClient_subscribe(*context, topic, qos);
	return rc;
}

void mqtt_destroy(MQTTClient *context) {
	MQTTClient_disconnect(*context, 1500L);
	MQTTClient_destroy(context);
	free(context);
}

/* *
 * Get Setting
 * 문자열로 된 세팅 값을 Key, Value 값으로 분리하여 리턴
 * example)
 * SET running_time 30 
 * [0]: running_time
 * [1]: 30
 * */
char **get_setting(char *setting_str, unsigned long len) {
	char *buf = (char*)malloc(sizeof(char)*len);
	char **ret = (char**)malloc(sizeof(char*)*2);
	char *ptr = NULL;
	unsigned long c_len = 0;
	if( setting_str == NULL ) return NULL;
	memset(buf, 0, len);
	strncpy(buf, setting_str, len);

	//
	ptr = strtok(buf, " ");
	c_len = strlen(ptr);
	ptr = strtok(NULL, " ");
	c_len = strlen(ptr);
	ret[0] = (char*)malloc(sizeof(char)*(c_len+1));
	memset(ret[0], 0, c_len+1);
	strcpy(ret[0], ptr);
	ptr = strtok(NULL, " ");
	c_len = strlen(ptr);
	ret[1] = (char*)malloc(sizeof(char)*(c_len+1));
	memset(ret[1], 0, c_len+1);
	strcpy(ret[1], ptr);
	
	//
	free(buf);
	//
	return ret;
}
/* *
 * Destroy Setting 
 * Get Setting 함수에서 얻은 세팅 값을 메모리 해제하는 함수 
 * */
void destroy_setting(char **setting) {
	int i = 0;
	for(i = 0 ; i < 2 ; i++ ){
		free(setting[i]);
	}
	free(setting);
}
