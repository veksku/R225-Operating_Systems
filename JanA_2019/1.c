#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <string.h>

#define check_error(expr, userMsg) \
	do {\
		if (!(expr)) {\
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv){
	check_error(argc == 3, "invalid input");
	
	time_t now = atoi(argv[1]);
	int brGodina = atoi(argv[2]);
	
	
	char* dan = NULL;
	char str[4];
	strncpy(str, ctime(&now),3);
	str[3] = '\0';
	
	switch(str[0]){
		case 'M':
			dan = malloc(11*sizeof(char));
			strcpy(dan, "ponedeljak");
			dan[10] = '\0';
			break;
		case 'T':
			if(str[1] == 'u'){
				dan = malloc(7*sizeof(char));
				strcpy(dan, "utorak");
				dan[6] = '\0';
				break;
			}
			else{
				dan = malloc(9*sizeof(char));
				strcpy(dan, "cetvrtak");
				dan[8] = '\0';
				break;
			}
		case 'W':
			dan = malloc(6*sizeof(char));
			strcpy(dan, "sreda");
			dan[5] = '\0';
			break;
		case 'F':
			dan = malloc(6*sizeof(char));
			strcpy(dan, "petak");
			dan[5] = '\0';
			break;
		case 'S':
			if(str[1] == 'a'){
				dan = malloc(7*sizeof(char));
				strcpy(dan, "subota");
				dan[6] = '\0';
				break;
			}
			else{
				dan = malloc(8*sizeof(char));
				strcpy(dan, "nedelja");
				dan[7] = '\0';
				break;
			}
		default:
			break;
	}
	
	struct tm* theTime = localtime(&now);
	check_error(theTime != NULL, "...");
	
	//printf("%d %d %d\n", theTime->tm_mday, theTime->tm_mon, theTime->tm_year);
	theTime->tm_year += brGodina;
	//printf("%d %d %d\n", theTime->tm_mday, theTime->tm_mon, theTime->tm_year);
	
	time_t newTime = mktime(theTime);
	
	struct tm* realTime = localtime(&newTime);
	
	int weekday = realTime->tm_wday;
	char* _dan = NULL;
	
	switch(weekday){
		case 0:
			_dan = malloc(8*sizeof(char));
			strcpy(_dan, "nedelja");
			_dan[7] = '\0';
			break;
		case 1:
			_dan = malloc(11*sizeof(char));
			strcpy(_dan, "ponedeljak");
			_dan[10] = '\0';
			break;
		case 2:
			_dan = malloc(7*sizeof(char));
			strcpy(_dan, "utorak");
			_dan[6] = '\0';
			break;
		case 3:
			_dan = malloc(6*sizeof(char));
			strcpy(_dan, "sreda");
			_dan[5] = '\0';
			break;
		case 4:
			_dan = malloc(9*sizeof(char));
			strcpy(_dan, "cetvrtak");
			_dan[8] = '\0';
			break;
		case 5:
			_dan = malloc(6*sizeof(char));
			strcpy(_dan, "petak");
			_dan[5] = '\0';
			break;
		case 6:
			_dan = malloc(7*sizeof(char));
			strcpy(_dan, "subota");
			_dan[6] = '\0';
			break;
		default:
			break;
	}
	
	printf("%s %s\n", dan, _dan);
	
	return 0;
	
	
	
	
	
	
	
	
	
	
}
