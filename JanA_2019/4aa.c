//Program kao argumente komandne linije dobija putanju fajla koji u svakoj liniji sadrzi po jednu komandu terminala oblika: komanda argument (dakle tacno jedan argument, razdvojeni jednom belinom, linija nije duza od 255 karaktera). Program pokrece svaku komandu u novom dete procesu, dohvata kroz pajp standardni izlaz komande i racuna ukupan broj bajtova koje je procitao za datu komandu. Ispisati na standardni izlaz iz roditelja komandu koja je proizvela najveci izlaz.

#define _GNU_SOURCE 700
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#define osErrorFatal(msg) osErrorFatalImpl((msg), __FILE__, __func__, __LINE__)
#define osAssert(expr, msg) \
	do{ \
		if(!(expr)) \
			osErrorFatal(msg); \
	}while(0)
	
#define MAX_SIZE (255)
#define RD_END (0)
#define WR_END (1)

void osErrorFatalImpl(const char* msg, const char* file, const char* func, const int lineNo){
	perror(msg);
	fprintf(stderr, "File: '%s'\nFunction: '%s'\nLine: '%d'\n", file, func, lineNo);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
	
	char maxLine[4096];
	
	osAssert( argc == 2, "wrong input");

	FILE* d = fopen(argv[1], "r");
	osAssert( d != NULL, "fopen failed");

	/*char buffer[MAX_SIZE];
	size_t bytesRead = read(fd, buffer, MAX_SIZE);
	buffer[bytesRead] = '\0';
	*/
	char* line = NULL;
	size_t size = 0;
	int procitano=0;
	errno = 0;
	int max;
	
	while( (procitano=getline(&line, &size, d)) != -1){
		
		line[procitano]= '\0';
		
		int pipes[2];
		osAssert(pipe(pipes) != -1, "pipe failed");
		
		pid_t childPid = fork();
		osAssert( childPid != -1, "fork failed");
	
		if(childPid == 0){
			close(pipes[RD_END]);
			char komanda[MAX_SIZE];
			char argument[MAX_SIZE];
			osAssert(dup2(pipes[WR_END], STDOUT_FILENO) != -1, "dup2 failed");
			//osAssert(dup2(pipes[WR_END], stderr);
			sscanf(line, "%s%s", komanda, argument);
			osAssert(execlp(komanda,komanda,argument, NULL) != -1, "exec failed");
			wait(0);
		}
		
		if(childPid > 0){
			close(pipes[WR_END]);
			
			char parLine[MAX_SIZE];
			int lineLen = 0;
			int length = 0;
			errno = 0;
			//FILE* f = fdopen(pipes[RD_END], "r");
			//osAssert(f != NULL, "fdopen failed");
			while( (lineLen = read(pipes[RD_END], parLine, sizeof(parLine))) > 0)
				length += lineLen;
			osAssert(0 == errno, "read failed");
			//printf("%s %s", komanda, argument);
			//while(getline(&parLine, &lineLen, f) != -1){
				//sscanf(parLine, "%s %s", komanda, argument);
				//length += strlen(parLine);
			//}
			if(length > max){
				max = length;
				strcpy(maxLine, line);
			}
			close(pipes[RD_END]);
			int status = 0;
			osAssert(waitpid(childPid, &status, 0) != -1, "wait failed");
			//fclose(f);
		}
		free(line);
	}
	osAssert(errno == 0, "getline failed");
	printf("%s", maxLine);
	fclose(d);
	return 0;
}
