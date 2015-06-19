#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include "daemon.h"
#include "include/MQTTClient.h"

extern void *sprinkler_init(void *data);

int main(int argc, char *argv[]) {
	MQTTClient *client;
	int rc = 0;
	int farm_fd = 0;
	char topic_status[16] = {0,};
	pthread_t sprinkler;
	struct daemon_info_t info;

	/*   
	pid_t pid;	
	pid = fork();
	if( pid == -1 ) {
		return -1;
	} else if( pid != 0 ) {
		exit(0);
	}

	// Daemon Setting
	signal(SIGHUP, SIG_IGN);
	close(0);
	close(1);
	close(2);

	chdir("/");
	setsid();

	*/

	// MQTT Setting
	client = (MQTTClient*)mqtt_create(argv[1], NULL);
	mqtt_topic(argv[1], "status", topic_status);
	mqtt_pub(client, "Daemon Start", topic_status, 0);

	// Open Device Driver
	farm_fd = farm_open();	

	//
	info.dev_name = argv[1];
	info.farm_fd = farm_fd;
	
	// Load Sub Module 
	pthread_create(&sprinkler, NULL, sprinkler_init, (void*)&info);
	pthread_detach(sprinkler);

	// Daemon Start
	while(1) {
		sleepu(1000);
	}

	return 0;
}
