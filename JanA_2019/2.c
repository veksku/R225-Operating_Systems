//Napisati program koji u beskonacnoj petlji ceka na signal, zatim ucitava ceo broj. Ako je pristigli signal SIGUSR1 program ispisuje apsolutnu vrednost tog broja, dok ako je signal SIGUSR2 program ispisuje kvadrat broja (svaki ispis je u novom redu). Dodatno, ako program primi SIGTERM potrebno je da se izadje iz petlje i zavrsi sa izvrsavanjem.
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include <signal.h>

#define osErrorFatal(msg) osErrorFatalImpl((msg), __FILE__, __func__, __LINE__)
#define osAssert(expr,msg) \
	do{\
		if(!(expr))\
			osErrorFatal(msg);\
	}while (0) \
	
void osErrorFatalImpl(const char* msg, const char* file, const char* func, const int lineNo){
	perror(msg);
	fprintf(stderr, "File: '%s'\nFunction: '%s'\nLine: '%d'\n", file, func, lineNo);
	exit(EXIT_FAILURE);
}

int sigValue = 0;
bool terminate = false;

void sigHandler (int sig){
	
	switch(sig){
		case SIGUSR1:
			scanf("%d", &sigValue);
			sigValue = abs(sigValue);
			printf("%d ", sigValue);
			break;
		case SIGUSR2:
			scanf("%d", &sigValue);
			sigValue = pow(sigValue, 2);
			printf("%d ", sigValue);
			break;
		case SIGTERM:
			terminate = true;
			break;
		default:
			break;
	}
}

int main(){
	
	osAssert(signal(SIGUSR1, sigHandler) != SIG_ERR, "signal handler");
	osAssert(signal(SIGUSR2, sigHandler) != SIG_ERR, "signal handler");
	osAssert(signal(SIGTERM, sigHandler) != SIG_ERR, "signal handler");
	
	do{
		pause();
	}while(!terminate);
	
	exit(EXIT_SUCCESS);
}
