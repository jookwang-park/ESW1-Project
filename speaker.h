
// Define Speaker IOCTL Commands
#define SPEAKER_PLAY	_IOWR(IOCTL_NUM, IOCTL_START_NUM+20, unsigned long *)
#define SPEAKER_STOP	_IOWR(IOCTL_NUM, IOCTL_START_NUM+21, unsigned long *)

// Define Speaker GPIO Pin Number
#define SPEAKER_GPIO	23

// Define Speaker Module Functions
int module_speaker_init(void);
void module_speaker_exit(void);
void module_speaker_play(void);
void module_speaker_stop(void);
