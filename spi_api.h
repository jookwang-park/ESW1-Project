const char *SPI_DEV = "/dev/spidev0.0";
const uint8_t SPI_BPW = 8;
const uint16_t SPI_DELAY = 0;

int SPI_DataRw(unsigned char *data, int len);
int SPI_SetupMode(int speed, int mode);
void SPI_On(int farm_fd);
void SPI_Off(int farm_fd);
int SPI_Read_Value(int farm_fd, unsigned char channel);
int SPI_Register_Handler(int farm_fd, unsigned char channel, int collect_time, int response_time, void (*handler)(int));
