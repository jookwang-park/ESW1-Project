#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "farm_api.h"

static int farm_fd = 0;

int farm_open() {
	farm_fd = open("/dev/"FARM_DEV, O_RDWR);
	return farm_fd;
}

void farm_close() {
	close(farm_fd);
}

void sleepm(unsigned int size) {
  struct timespec sleeper, dummy ;

  sleeper.tv_sec = (time_t)(size / 1000) ;
  sleeper.tv_nsec = (long)(size % 1000) * 1000000 ;

  nanosleep(&sleeper, &dummy) ;
}


void sleepu_hard (unsigned int size) {
  struct timeval tNow, tLong, tEnd ;

  gettimeofday(&tNow, NULL) ;
  tLong.tv_sec = size / 1000000 ;
  tLong.tv_usec = size % 1000000 ;
  timeradd(&tNow, &tLong, &tEnd) ;

  while (timercmp(&tNow, &tEnd, <))
    gettimeofday(&tNow, NULL) ;
}

void sleepu (unsigned int size) {
  struct timespec sleeper ;

  if (size == 0)
    return ;
  else if (size  < 100)
    sleepu_hard(size) ;
  else
  {
    sleeper.tv_sec  = 0 ;
    sleeper.tv_nsec = (long)(size * 1000) ;
    nanosleep (&sleeper, NULL) ;
  }
}


