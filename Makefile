PORT := 10000
USER := papimomi
HOST := 172.16.46.133
HOST_PATH := ~/farm
KDIR := /Volumes/linux-rpi
  
obj-m   := farm_mod.o
farm_mod-objs := farm.o motor.o led.o speaker.o spi.o dht.o

default:
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- modules 
copy:
	scp -P $(PORT) farm_mod.ko $(USER)@$(HOST):$(HOST_PATH)
	scp -P $(PORT) *.sh $(USER)@$(HOST):$(HOST_PATH)
	scp -P $(PORT) *.out $(USER)@$(HOST):$(HOST_PATH)
app:
	arm-none-linux-gnueabi-gcc -o daemon.out ./daemon/main.c ./daemon/util.c ./daemon/sprinkler.c motor_api.c farm_api.c led_api.c speaker_api.c spi_api.c dht_api.c -lpthread -L daemon/lib -lpaho-mqtt3c -lpaho-mqtt3a ./daemon/light.c -g -ggdb
test:
	arm-none-linux-gnueabi-gcc -o test.out test.c motor_api.c farm_api.c led_api.c speaker_api.c spi_api.c dht_api.c -lpthread
clean:
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- clean 
