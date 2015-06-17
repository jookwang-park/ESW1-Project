PORT := 10000
USER := papimomi
HOST := 172.16.47.79
HOST_PATH := ~/farm
KDIR := /Volumes/linux-rpi
  
obj-m   := farm_mod.o
farm_mod-objs := farm.o motor.o

default:
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- modules 
copy:
	scp -P $(PORT) farm_mod.ko $(USER)@$(HOST):$(HOST_PATH)
	scp -P $(PORT) *.sh $(USER)@$(HOST):$(HOST_PATH)
	scp -P $(PORT) *.out $(USER)@$(HOST):$(HOST_PATH)
test:
	arm-none-linux-gnueabi-gcc -o test.out test.c motor_api.c farm_api.c
clean:
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- clean 
