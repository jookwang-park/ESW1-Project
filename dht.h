
// Define DHT IOCTL Commands
#define DHT_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+60, unsigned long *)
#define DHT_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+61, unsigned long *)
#define DHT_SET_INPUT	_IOWR(IOCTL_NUM, IOCTL_START_NUM+62, unsigned long *)
#define DHT_SET_OUTPUT	_IOWR(IOCTL_NUM, IOCTL_START_NUM+63, unsigned long *)
#define DHT_GET_DATA	_IOWR(IOCTL_NUM, IOCTL_START_NUM+64, unsigned long *)

// Define DHT GPIO Pin Number
#define DHT_GPIO	12

// Define DHT Module Functions
int module_dht_init(void);
void module_dht_exit(void);
void module_dht_on(void);
void module_dht_off(void);
void module_dht_set_input(void);
void module_dht_set_output(void);
int module_dht_get_value(void);
