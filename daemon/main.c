#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	pid_t pid;
	
	pid = fork();
	if( pid == -1 ) {
		return -1;
	} else if( pid != 0 ) {
		exit(0);
	}

	// Daemon Setting
	signal(SIGHUP, SIG_IGN);
	close(0);
	close(1);
	close(2);

	chdir("/");
	setsid();

	// Daemon Start
	while(1) {
		usleep(1000);
	}

	return 0;
}
