#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void logging(char *filename, char str[255]) {
	FILE *log = NULL;
	char path[255] = {0,};
	char log_str[255] = {0,};
	sprintf(path, "/tmp/farm/%s", filename);
	mkdir("/tmp/farm");
	log = fopen(path, "a");
	sprintf(log_str, "%s\n", str);
	fprintf(log, log_str);
	fclose(log);
}
