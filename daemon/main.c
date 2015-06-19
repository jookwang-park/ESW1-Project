#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, int argv) {
	pid_t pid;

	if( (pid = fork()) < 0 ) {
		exit(0);
	} else if( pid != 0 ) {
		exit(0);
	}

	// Daemon Setting
	signal(SIGNUP, SIG_IGN);
	close(STDIN);
	close(STDOUT);
	close(STDERR);

	chdir("/");
	setsid();

	// Daemon Start
	while(1) {

		usleep(1000);
	}

	return 0;
}
