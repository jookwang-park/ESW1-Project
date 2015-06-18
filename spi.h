
// Define SPI IOCTL Commands
#define SPI_OFF	_IOWR(IOCTL_NUM, IOCTL_START_NUM+30, unsigned long *)
#define SPI_ON	_IOWR(IOCTL_NUM, IOCTL_START_NUM+31, unsigned long *)

// Define SPI GPIO Pin Number (CS0)
#define SPI_GPIO	6

// Define SPI Module Functions
int module_spi_init(void);
void module_spi_exit(void);
void module_spi_on(void);
void module_spi_off(void);
