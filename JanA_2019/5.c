//Program kao argumente komandne linije prima putanju do fajla i rec i treba da ispise pozicije i tip katanca za sva zakljucana pojavljivanja ove reci u fajlu (svaku u novom redu). Maksimalna duzina reci je 255 karaktera.
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
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
#define MAX_SIZE_BUF (8192)

void osErrorFatalImpl(const char* msg, const char* file, const char* func, const int lineNo){
	perror(msg);
	fprintf(stderr, "File: '%s'\nFunction: '%s'\nLine: '%d'\n", file, func, lineNo);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
	
	osAssert(argc == 3, "invalid input");
	
	char* rec;
	int duzina = strlen(argv[2]);
	rec = malloc(duzina + 1);
	osAssert(rec != NULL, "malloc failed");
	strcpy(rec, argv[2]);
	
	int fd = open(argv[1], O_RDWR);
	osAssert(fd != -1, "open failed");
	
	char buffer[MAX_SIZE_BUF];
	int bytesRead = read(fd, buffer, MAX_SIZE_BUF);
	osAssert(bytesRead != -1, "read failed");
	buffer[bytesRead] = '\0';
	
	struct flock lock;
	
	int ptr=0;
	/*while(buffer[ptr] != '\0'){
		if(buffer[ptr] == rec[0]){
			if(!(strncmp(&buffer[ptr], &rec[0], duzina))){
				lock.l_type = F_WRLCK;
				lock.l_whence = SEEK_SET;
				lock.l_start = ptr;
				lock.l_len = duzina;
				osAssert(fcntl(fd, F_GETLK, &lock) != -1, "flock failed");
				switch(lock.l_type){
					case F_UNLCK:
						break;
					case F_RDLCK:
						printf("%d r", ptr);
						break;
					case F_WRLCK:
						printf("%d w", ptr);
						break;
					default:
						break;
				}
				ptr += duzina;
			}
			else
				ptr++;
		}
		else
			ptr++;
	}*/
	for(int i=0; buffer[i] != '\0'; i++){
		if(!(strncmp(&buffer[i], rec, duzina))){
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = i;
			lock.l_len = duzina;
			osAssert(fcntl(fd, F_GETLK, &lock) != -1, "flock failed");
			switch(lock.l_type){
				case F_RDLCK:
					printf("%d r\n", i);
					break;
				case F_WRLCK:
					printf("%d w\n", i);
					break;
				default:
					break;
			}
		}
	}
}
