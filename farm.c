#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include "farm.h"
#include "motor.h"
#include "led.h"

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

	printk("Goodbye Farm!\n");
}

/* *
 * Module Setting
 * */
module_init(farm_init);
module_exit(farm_exit);
