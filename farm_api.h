// Define Character Device Name
#define FARM_DEV	"farm_dev"

// Define IOCTL Commands
#define IOCTL_START_NUM	0x80
#define IOCTL_NUM	'z'

// Define Motor IOCTL Commands
#define MOTOR_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM + 10, unsigned long *)
#define MOTOR_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM + 11, unsigned long *)


