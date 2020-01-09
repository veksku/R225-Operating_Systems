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
int i=0;

int main(int argc, char** argv){
	
	osAssert( argc == 2, "wrong input");

	FILE* d = fopen(argv[1], "r");
	osAssert( d != NULL, "fopen failed");

	char* line = NULL;
	size_t size = 0;
	int procitano=0;
	errno = 0;
	
	int fd = open("errors.txt", O_RDWR | O_CREAT, 0644);
	char lines[20][4094];
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

			sscanf(line, "%s%s", komanda, argument);

			osAssert(dup2(pipes[WR_END], STDOUT_FILENO) != -1, "dup2 failed");
			osAssert(dup2(pipes[WR_END], STDERR_FILENO) != -1, "dup2 stderr");
			osAssert(execlp(komanda,komanda,argument, NULL) != -1, "exec failed");
			
			wait(0);
		}
		
		if(childPid > 0){
			close(pipes[WR_END]);

			int status = 0;
			osAssert(waitpid(childPid, &status, 0) != -1, "wait failed");
			if(status != 0){
				osAssert( read(pipes[RD_END], lines[i], 4094) != -1, "read failed");
				dprintf(fd, "%s", lines[i]);
				i++;
			}
		}
		free(line);
	}
	fclose(d);
	close(fd);
	return 0;
}
