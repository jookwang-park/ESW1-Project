
// Define LED IOCTL Commands
#define LED_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+15, unsigned long *)
#define LED_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+16, unsigned long *)

// Define LED GPIO Pin Number
#define LED_GPIO	5

// Define LED Module Functions
int module_led_init(void);
void module_led_exit(void);
void module_led_on(void);
void module_led_off(void);
