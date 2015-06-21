//define Motor IOCTL Command
#define	PIR_SET_SENS	_IOWR(IOCTL_NUM, IOCTL_START_NUM+40, unsigned long *)
#define	PIR_GET_DATA	_IOWR(IOCTL_NUM, IOCTL_START_NUM+41, unsigned long *)

//define PIR GPIO Pin number
#define	PIR_GPIO	25

//define PIR module
int module_pir_init(void);
void module_pir_exit(void);
void module_pir_set_sens(int sen);
int module_pir_get_data(void);
