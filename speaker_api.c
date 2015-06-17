#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "farm_api.h"

struct speaker_data {
	int farm_fd;
	int delay;
	int us;
};

static pthread_t speaker_thread;
static int speaker_thread_id;

static void *play(void *data) {
	struct speaker_data *sp_data = (struct speaker_data*)data;
	int delay = sp_data->delay;
	int us = sp_data->us;
	int farm_fd = sp_data->farm_fd;
	int i = 0 ;
	free(sp_data);
	for(i = 0 ; i < delay ; i ++ ) {
		ioctl(farm_fd, SPEAKER_PLAY, 0);
		usleep(us);
		ioctl(farm_fd, SPEAKER_STOP, 0);
		usleep(us);
	}
}

void speaker_play(int farm_fd, int delay, int us) {
	struct speaker_data *data = (struct speaker_data*)malloc(sizeof(struct speaker_data));
	data->farm_fd = farm_fd;
	data->delay = delay;
	data->us = us;

	speaker_thread_id = pthread_create(&speaker_thread, NULL, play, (void*)data);
	pthread_detach(speaker_thread_id);
}

void speaker_stop(int farm_fd) {
	ioctl(farm_fd, SPEAKER_STOP, 0);
}
