#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(void) {
	char cmd[255] = {0};
	int me = fork();

	sprintf(cmd, "echo child %d: and I can run subprocesses!", me);
	while (1) {
		printf("child %d: i am alive!\n", me);
		system(cmd);
		sleep(2);
	}
}
