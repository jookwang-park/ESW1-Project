#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");

/* *
 * Farm Module Init
 * */
static int __init farm_init(void) {
	return 0;
}

/* *
 * Farm Module Exit
 * */
static void __exit farm_exit(void) {
	
}

/* *
 * Module Setting
 * */
module_init(farm_init);
module_exit(farm_exit);
