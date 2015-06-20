#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include "farm.h"
#include "motor.h"
#include "led.h"
#include "speaker.h"
#include "spi.h"
#include "dht.h"

MODULE_LICENSE("GPL");

static dev_t dev_num = 0;
static struct cdev *cd_cdev = NULL;

static int farm_open(struct inode *, struct file *);
static int farm_release(struct inode *, struct file *);
static long farm_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations farm_fops = {
	.open = farm_open,
	.release = farm_release,
	.unlocked_ioctl = farm_ioctl
};

static int farm_open(struct inode *inode, struct file *file) {
	return 0;
}

static int farm_release(struct inode *inode, struct file *file) {
	return 0;
}

static long farm_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	int value = 0;
	unsigned long copy_size = 0;
	switch(cmd) {
		case MOTOR_OFF:
			module_motor_off();
			break;
		case MOTOR_ON:
			module_motor_on();
			break;
		case LED_OFF:
			module_led_off();
			break;
		case LED_ON:
			module_led_on();
		case SPEAKER_PLAY:
			module_speaker_play();
			break;
		case SPEAKER_STOP:
			module_speaker_stop();
			break;
		case SPI_ON:
			module_spi_on();
			break;
		case SPI_OFF:
			module_spi_off();
			break;
		case DHT_ON:
			module_dht_on();
			break;
		case DHT_OFF:
			module_dht_off();
			break;
		case DHT_SET_INPUT:
			module_dht_set_input();
			break;
		case DHT_SET_OUTPUT:
			module_dht_set_output();
			break;
		case DHT_GET_DATA:
			value = module_dht_get_value();
			copy_size = copy_to_user((int*)arg, (int*)&value, sizeof(value));
			break;
		default:
			break;
	}
	return 0;
}

/* *
 * Farm Module Init
 * */
static int __init farm_init(void) {
	// Initialize Character Device
	int ret = 0;
	alloc_chrdev_region(&dev_num, 0, 1, FARM_DEV);
	cd_cdev = cdev_alloc();
	cdev_init(cd_cdev, &farm_fops);
	ret = cdev_add(cd_cdev, dev_num, 1);
	if( ret < 0 ) return -1;
	
	// Initialize Motor Device
	ret = module_motor_init();	
	if( ret < 0 ) return -1;

	// Initialize LED Device
	ret = module_led_init();	
	if( ret < 0 ) return -1;

	// Initialize SPEAKER Device
	ret = module_speaker_init();	
	if( ret < 0 ) return -1;

	// Initialize SPI Device
	ret = module_spi_init();	
	if( ret < 0 ) return -1;

	// Initialize DHT Device
	ret = module_dht_init();	
	if( ret < 0 ) return -1;

	printk("Hello Farm!\n");
	return 0;
}

/* *
 * Farm Module Exit
 * */
static void __exit farm_exit(void) {
	// Destroy Character Device
	cdev_del(cd_cdev);
	unregister_chrdev_region(dev_num, 1);

	// Destroy Other Modules
	module_motor_exit();
	module_led_exit();
	module_speaker_exit();
	module_spi_exit();
	module_dht_exit();
	printk("Goodbye Farm!\n");
}

/* *
 * Module Setting
 * */
module_init(farm_init);
module_exit(farm_exit);
