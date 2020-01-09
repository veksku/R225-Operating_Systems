#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <ctype.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

#define check_error(expr,msg)\
	do{\
		if(!(expr)){\
			perror(msg);\
			exit(EXIT_FAILURE);\
		}\
	} while(0)
	
#define MAX_SIZE 256	
	
int main(int argc, char** argv){
	
	check_error( argc == 2, "invalid input");
	
	int fd = open(argv[1], O_RDWR);
	
	struct flock lock;
	
	char tarabe[5] = "####";
	char buffer[MAX_SIZE];
	errno = 0;
	ssize_t bytesRead = read(fd, buffer, MAX_SIZE);
	check_error( 0 == errno, "read failed");
	buffer[bytesRead]= 0;
	
	int offset=0;
	
	for(offset = 0; buffer[offset] != EOF; offset++){
		if(isdigit(buffer[offset])){
			if(offset != 0){
				if(buffer[offset-1] == '-'){
					int i=1;
					for(i=1; buffer[offset+i] != EOF; i++){
						if( isdigit(buffer[offset+i]))
							i++;
						else
							break;
					}
					offset += i-1;
				}
				else{
					int i=1;
					while(isdigit(buffer[offset+i]))
						i++;
					if(i==4){
						lock.l_type = F_WRLCK;
						lock.l_whence = SEEK_SET;
						lock.l_start = offset;
						lock.l_len = 4;
						check_error(lseek(fd, offset, SEEK_SET) != (off_t)-1, "lseek failed");
						check_error(fcntl(fd, F_GETLK, &lock) != -1, "getlock failed");
						switch(lock.l_type){
							case F_RDLCK:
								break;
							case F_WRLCK:
								break;
							case F_UNLCK:
								check_error(write(fd, tarabe, 4) != -1, "write failed");
								check_error(fcntl(fd, F_SETLK, &lock) != -1, "setlock failed");
							default:
								break;
						}
					}
					offset += i-1;
				}
			}
			else{
				int i=1;
				while(isdigit(buffer[offset+i]))
					i++;
				if(i==4){
					lock.l_type = F_WRLCK;
					lock.l_whence = SEEK_SET;
					lock.l_start = offset;
					lock.l_len = 4;
					check_error(lseek(fd, offset, SEEK_SET) != (off_t)-1, "lseek failed");
					check_error(fcntl(fd, F_GETLK, &lock) != -1, "getlock failed");
					switch(lock.l_type){
						case F_RDLCK:
							break;
						case F_WRLCK:
							break;
						case F_UNLCK:
							check_error(write(fd, tarabe, 4) != -1, "write failed");
							check_error(fcntl(fd, F_SETLK, &lock) != -1, "setlock failed");
						default:
							break;
					}
				}
				offset += i-1;
			}
		}
	}
	exit(EXIT_SUCCESS);
}
