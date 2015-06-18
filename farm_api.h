// Define Character Device Name
#define FARM_DEV	"farm_dev"

// Define IOCTL Commands
#define IOCTL_START_NUM	0x80
#define IOCTL_NUM	'z'

// Define Motor IOCTL Commands
#define MOTOR_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM + 10, unsigned long *)
#define MOTOR_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM + 11, unsigned long *)

// Define LED IOCTL Commands
#define LED_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+15, unsigned long *)
#define LED_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+16, unsigned long *)

// Define Speaker IOCTL Commands
#define SPEAKER_PLAY	_IOWR(IOCTL_NUM, IOCTL_START_NUM+20, unsigned long *)
#define SPEAKER_STOP	_IOWR(IOCTL_NUM, IOCTL_START_NUM+21, unsigned long *)

// Define SPI IOCTL Commands
#define SPI_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+30, unsigned long *)
#define SPI_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+31, unsigned long *)


