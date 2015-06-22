
// Define UltraSonic IOCTL Commands
#define ULTRASONIC_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+50, unsigned long *)
#define ULTRASONIC_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+51, unsigned long *)
#define ULTRASONIC_ECHO	_IOWR(IOCTL_NUM, IOCTL_START_NUM+52, unsigned long *)

// Define UltraSonic GPIO Pin Number
#define ULTRASONIC_GPIO_IN	19
#define ULTRASONIC_GPIO_OUT	13

// Define Motor Module Functions
int module_ultrasonic_init(void);
void module_ultrasonic_exit(void);
void module_ultrasonic_trigger_on(void);
void module_ultrasonic_trigger_off(void);
int module_ultrasonic_get_echo(void);
