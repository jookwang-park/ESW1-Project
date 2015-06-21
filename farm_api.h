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

//define Motor IOCTL Command
#define	PIR_SET_SENS	_IOWR(IOCTL_NUM, IOCTL_START_NUM+40, unsigned long *)
#define	PIR_GET_DATA	_IOWR(IOCTL_NUM, IOCTL_START_NUM+41, unsigned long *)

// Define DHT IOCTL Commands
#define DHT_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+60, unsigned long *)
#define DHT_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+61, unsigned long *)
#define DHT_SET_INPUT	_IOWR(IOCTL_NUM, IOCTL_START_NUM+62, unsigned long *)
#define DHT_SET_OUTPUT	_IOWR(IOCTL_NUM, IOCTL_START_NUM+63, unsigned long *)
#define DHT_GET_DATA	_IOWR(IOCTL_NUM, IOCTL_START_NUM+64, unsigned long *)


