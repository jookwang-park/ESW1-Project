#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "farm.h"
#include "pir.h"

MODULE_LICENSE("GPL");

#define	HIGH	2
#define MEDIUM	5
#define	LOW	10

static int irq_num;
static int cnt = 0;
static int sens = MEDIUM;	//default
static int is_detect = 0;

static void module_pir_set_sensivity(int sen){

	if(sen == 1){
		cnt  = 0;
		sens = HIGH;
	}
	else if(sen == 2){
		cnt  = 0;
		sens = MEDIUM;
	}
	else{
		cnt  = 0;
		sens = LOW;
	}
}

void module_pir_set_sens(int sen) {
	module_pir_set_sensivity(sen);
}

int module_pir_get_data(void) {
	int tmp = is_detect;
	if( is_detect == 1 ) is_detect = 0;
	return tmp;
}

static irqreturn_t detect_sparrow(int irq, void* data){
	int status = 0;
	status = gpio_get_value(PIR_GPIO);
	if(status == 0){// something enters
		cnt++;
	}else if(status == 1){	//something leaves
		cnt++;
	}


	if(cnt == sens){
		cnt = 0;
		is_detect = 1;
		//sends to server mqtt message like "speaker works"
		
	}
	return IRQ_HANDLED;
}

int module_pir_init(void) {
	int ret = 0;
	gpio_request_one(PIR_GPIO, GPIOF_IN, "pir");
	irq_num = gpio_to_irq(PIR_GPIO);

	ret = request_irq(irq_num, detect_sparrow, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "sensor_pir", NULL);
	if(ret){
		printk(KERN_ERR "UNABLE TO REQUEST IRQ : %d\n", ret);
		free_irq(irq_num, NULL);
	}
	return 0;
}

void module_pir_exit(void) {
	free_irq(irq_num, NULL);
	gpio_free(PIR_GPIO);	
}
