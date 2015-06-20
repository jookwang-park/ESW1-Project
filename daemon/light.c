#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <unistd.h>
#include <pthread.h>

#include <time.h>


#include "daemon.h"
#include "include/MQTTClient.h"

/*
func





*/
void light_handler(int avg);


void light_state();
void light_config();
void light_start();
void light_running();
void light_end();

void light_func_test(int tic, int ticinterval, int avginterval);



enum LIGHT_STATE {

	INIT,
	CONFIG,
	START,
	RUNNING,
	END,
	TEST
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


static char *dev_name = NULL;
static int farm_fd = 0;


static struct daemon_info_t info;


static int curavglux = 0;
static int prevavglux = 0;
static int deltalux = 0;

static int count = 0;
static int ticcnt = 0;


void *light_init(void *data) {


	info = *((struct daemon_info_t*)data);

	farm_fd = info.farm_fd;
	dev_name = info.dev_name;




	state = TEST;
	while(1) {
		light_state();
		usleep(100000);
	}

}

// tic = sec, ticinterval = sec, avginterval = min
void light_func_test(int tic, int ticinterval, int avginterval){
	
	//ticcnt 만큼 호출하면 avginterval 만큼 지나있다.

	// avginterval 마다 의 변화량의 틱을 알기위해
	// 체크해야하는 avginterval / tic /ticinterval * 60

	ticcnt = (int)(avginterval * 60 / tic / ticinterval);

	//SPI_Regist~ : tic(sec)마다 lux값을 더한후 ticinterval(sec)으로 나눠서 평균값을 구함.
	SPI_Register_Handler(farm_fd, 0, tic, ticinterval, light_handler);


	//필요한것은 위에서 받아온 값을 가지고있다가 avginterval 이 지날때마다 값의 변화를 겟.


	state = TEST;

}


void light_config(){




}

void light_start(){

}

void light_running(){

}

void light_end(){

}


void light_state() {
	switch(state) {
		
		case TEST:
		printf("case:test\n");
		//1초마다 습도를 가져오고
		//10개의 샘플
		// ==> 10초 동안의 평균
		//1 분마다 변화량 체크
		//parameter 
		// sec, sec, min

		light_func_test(1, 10, 1);
		break;
		

		case CONFIG:

		break;

		case START:

		break;

		case RUNNING:

		break;

		case END:

		break;
	}


}




void light_handler(int avg) {

	time_t timer;
	struct tm *t;

  timer = time(NULL); // 현재 시각을 초 단위로 얻기

  t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기


  // Unix Time  timer // 1970-01-01 00:00:00 start.
  // year : t->tm_year + 1900);
  // mon : t->tm_mon + 1
  // t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec

  if(t->tm_mon>=3 && t->tm_mon<=5){
  	season = SPRING;
  	printf("SPRING\n");

  	if(t->tm_hour <= 18 && t->tm_hour >= 5){
  		nightday = 1;
  	}else{
  		nightday = 0;
  	}
  } else if(t->tm_mon>=6 && t->tm_mon<=8){
  	season = SUMMER;
  	printf("SUMMER\n");

  	if(t->tm_hour <= 19 && t->tm_hour >= 5){
  		nightday = 1;
  	}else{
  		nightday = 0;
  	}

  } else if(t->tm_mon>=9 && t->tm_mon<=11){
  	season = FALL;
  	printf("FALL\n");

  	if(t->tm_hour <= 18 && t->tm_hour >= 5){
  		nightday = 1;
  	}else{
  		nightday = 0;
  	}
  	
  }  else if(t->tm_mon>=12 || t->tm_mon<=2){
  	season = WINTER;
  	printf("WINTER\n");

  	if(t->tm_hour <= 17 && t->tm_hour >= 6){
  		nightday = 1;
  	}else{
  		nightday = 0;
  	}

  }


  // 추가되면 사용할 변수. 
  // curavglux = avg;

  if(count%ticcnt==0){

  	deltalux = prevavglux - avg;

  	printf("deltalux = %d\n", deltalux);

  	prevavglux = avg;
  	count = 0;


	// printf("count : %d\n", count);

  	//nightday==1  day
  	if(nightday == 1){
  		printf("day\n");
  		if(deltalux > 900){
  			printf("rainy\n");
  		} else if(deltalux < -900) {
  			printf("sunny\n");
  		} else {
  			printf("soso\n");
  		}

  	//nightday==0  night
  	}else{
  		printf("night\n");

  	}
  }



  count++;

	// printf("count : %d\n", count);
  printf("Light Value Average: %d\n", avg);
}

