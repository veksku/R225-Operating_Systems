#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#define check_error(expr, userMsg) \
	do {\
		if (!(expr)) {\
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int skor = 0;
bool shouldTerminate = false;

void sigHandler(int sig){
	switch(sig){
		case SIGUSR1:
			skor++;
			break;
		case SIGUSR2:
			skor += 2;
			break;
		case SIGINT:
			skor -= 4;
			break;
		case SIGTERM:
			shouldTerminate = true;
			break;
		default:
			break;
	}
}

int main(int argc, char** argv){
	
	check_error(signal(SIGUSR1, sigHandler) != SIG_ERR, "signal failed");
	check_error(signal(SIGUSR2, sigHandler) != SIG_ERR, "signal failed");
	check_error(signal(SIGINT, sigHandler) != SIG_ERR, "signal failed");
	check_error(signal(SIGTERM, sigHandler) != SIG_ERR, "signal failed");
	
	do{
		pause();
	}while(!shouldTerminate);
	
	printf("%d\n", skor);
	
	exit(EXIT_SUCCESS);
}
