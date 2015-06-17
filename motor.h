
// Define Motor IOCTL Commands
#define MOTOR_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+10, unsigned long *)
#define MOTOR_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+11, unsigned long *)

// Define Motor GPIO Pin Number
#define MOTOR_GPIO	21

// Define Motor Module Functions
int module_motor_init(void);
void module_motor_exit(void);
void module_motor_on(void);
void module_motor_off(void);
