#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "farm_api.h"

static int farm_fd = 0;
static uint64_t epochMicro ;

int farm_open() {
	struct timeval tv ;
	gettimeofday (&tv, NULL) ;
	epochMicro = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec) ;
	farm_fd = open("/dev/"FARM_DEV, O_RDWR);
	return farm_fd;
}

void farm_close() {
	close(farm_fd);
}

//1000 = 1 sec
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

//1000000 = 1sec
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

unsigned int micros (void)
{
  struct timeval tv ;
  uint64_t now ;

  gettimeofday (&tv, NULL) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec ;

  return (uint32_t)(now - epochMicro) ;
}

