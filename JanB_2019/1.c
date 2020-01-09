#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <string.h>

#define check_error(expr,msg)\
	do{\
		if(!(expr)){\
			perror(msg);\
			exit(EXIT_FAILURE);\
		}\
	} while(0)

#define check_thread(pthreadErr, msg) \
	do{ \
		int _pthreadErr = pthreadErr;\
		if(_pthreadErr){\
			errno = _pthreadErr;\
			check_error(0, msg);\
		}\
	} while(0)
	
char* ispisimesec(int month){
	
	char* mesec=NULL;
	int length=0;
	switch(month){
		case 0:
			length = strlen("januar")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "januar");
			mesec[length] = '\0';
			break;
		case 1:
			length = strlen("februar")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "februar");
			mesec[length] = '\0';
			break;
		case 2:
			length = strlen("mart")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "mart");
			mesec[length] = '\0';
			break;
		case 3:
			length = strlen("april")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "april");
			mesec[length] = '\0';
			break;
		case 4:
			length = strlen("maj")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "maj");
			mesec[length] = '\0';
			break;
		case 5:
			length = strlen("jun")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "jun");
			mesec[length] = '\0';
			break;
		case 6:
			length = strlen("jul")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "jul");
			mesec[length] = '\0';
			break;
		case 7:
			length = strlen("avgust")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "avgust");
			mesec[length] = '\0';
			break;
		case 8:
			length = strlen("septembar")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "septembar");
			mesec[length] = '\0';
			break;
		case 9:
			length = strlen("oktobar")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "oktobar");
			mesec[length] = '\0';
			break;
		case 10:
			length = strlen("novembar")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "novembar");
			mesec[length] = '\0';
			break;
		case 11:
			length = strlen("decembar")+1;
			mesec = malloc(length*sizeof(char));
			strcpy(mesec, "decembar");
			mesec[length] = '\0';
			break;
		default:
			break;
	}
	return mesec;
}	
	
int main(int argc, char** argv){
	check_error( argc == 3, "wrong input");
	
	time_t t = atoi(argv[1]);
	int brDana = atoi(argv[2]);
	
	struct tm* unetoVreme = localtime(&t);
	check_error( unetoVreme != NULL, "..");
	
	int month = unetoVreme->tm_mon;
	
	printf("%s ", ispisimesec(month));
	
	unetoVreme->tm_mday += brDana;
	
	t = mktime(unetoVreme);
	
	unetoVreme = localtime(&t);
	
	month = unetoVreme->tm_mon;
	
	printf("%s\n", ispisimesec(month));
	
	exit(EXIT_SUCCESS);
}
