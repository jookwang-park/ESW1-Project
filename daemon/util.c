#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/MQTTClient.h"

#define	MQTT_ADDRESS		"tcp://brainguys.net:1883"
#define	MQTT_TIMEOUT		10000L

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

MQTTClient *mqtt_create(char *dev_name) {
	MQTTClient *context = (MQTTClient*)malloc(sizeof(MQTTClient));
	MQTTClient_create(context, MQTT_ADDRESS, dev_name, MQTTCLIENT_PERSISTENCE_NONE, NULL);	
	return context;
}

void mqtt_topic(char *dev_name, char *topic, char *dest) {
	char buf[255] = {0,};
	sprintf(buf, "%s/%s", dev_name, topic);
	strncpy(dest, buf, strlen(buf));
}

int mqtt_connect(MQTTClient *client) {
	int rc = 0;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.keepAliveInterval = 3;
	conn_opts.cleansession = 1;
	rc = MQTTClient_connect(*client, &conn_opts);
	if( rc != MQTTCLIENT_SUCCESS )  {
		return -1;
	}
	return 0;
}

int mqtt_pub(char *dev_name, MQTTClient *context, char *payload, char *topic, int qos) {
	MQTTClient client = *context;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	int rc = 0;
	if( !MQTTClient_isConnected(client) ) {
		rc = mqtt_connect(context);
	}
	pubmsg.payload = payload;
	pubmsg.payloadlen = strlen(payload);
	pubmsg.qos = qos;
	pubmsg.retained = 0;
	rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token);
	rc = MQTTClient_waitForCompletion(client, token, MQTT_TIMEOUT);
	rc = MQTTClient_disconnect(client, 1500L);
	return rc;
}

int mqtt_sub(MQTTClient *context, char *topic, int qos) {
	return MQTTClient_subscribe(*context, topic, qos);
}

int mqtt_recv(MQTTClient *context, void (*handler)(int, char*)) {
	char *topicName = NULL;
	int receiveLen = 0;
	int rc = 0;
	MQTTClient_message *message = NULL;
	MQTTClient_receive(*context, &topicName, &receiveLen, &message, 150000L);
	if( message ) {
		rc = 0;
		handler(receiveLen, (char*)message->payload);
		MQTTClient_freeMessage(&message);
	} else {
		rc = -1;
	}
	MQTTClient_free(topicName);
	return rc ;
}

void mqtt_destroy(MQTTClient *context) {
	MQTTClient_destroy(context);
	free(context);
}
