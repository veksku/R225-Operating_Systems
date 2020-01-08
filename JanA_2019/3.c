//Sa standardnog ulaza se ucitavaju celi brojevi m, n i k pa zatim m vektora duzine n (elementi vektora su realni brojevi jednostruke tacnosti tj. float). Potrebno je pokrenuti k niti od kojih svaka racuna normu za m/k vektora (predpostaviti m%k == 0). Niti vracaju lokalnu maksimalnu izracunatu normu kao povratnu vrednost koja se dohvata u main funkciji. Iz main funkcije se ispisuje indeks niti koja je izracunala maksimalnu normu kao i vrednost te norme. Pomoc: norma vektora je koren zbira kvadrata elemenata vektora. Maksimum normi koje su niti vratile mora da se racuna u main funkciji!

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <pthread.h>
#include <math.h>

#define check_error(expr, userMsg)\
	do { \
		if (!(expr)) {\
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while(0)

#define check_errorThread(pthreadErr, userMsg)\
	do {\
		int _pthreadErr = pthreadErr; \
		if (_pthreadErr) { \
			errno = _pthreadErr; \
			check_error(false, userMsg); \
		} \
	} while(0)

double* argVektori;
int m,n,k;

typedef struct{
	int threadNo;
} inputArg;

typedef struct{
	float max;
} outputArg;

void* threadFunc(void* arg){
	inputArg* threadArg = (inputArg*)arg;
	
	outputArg* retVal = malloc(sizeof(outputArg));
	check_error( retVal != NULL, "malloc failed retVal");
	
	int count = threadArg->threadNo - 1; //jer se tredovi broje pocevsi od 1, ne 0
	float maks=0, norma=0;
	
	for(int i=count*(m/k); i<(count+1)*(m/k); i++){
		for(int j=0; j<n; j++){
			norma += argVektori[i + i*(m-1) + j]*argVektori[i + i*(m-1) + j];
		}
		norma = sqrt(norma);
		if(maks < norma)
			maks = norma;
		norma = 0;
	}
	retVal->max = maks;
	return retVal;
}

int main(int argc, char** argv){
	check_error(argc == 1, "bad input");
	
	scanf("%d %d %d", &m, &n, &k);
	
	argVektori = malloc (n*m*sizeof(float));
	check_error( argVektori != NULL, "malloc failed vektori");
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			scanf("%lf", &argVektori[i + i*(m-1) + j]);
		}
	}
	
	pthread_t* tids = malloc(k*sizeof(pthread_t));
	check_error(tids != NULL, "malloc failed tids");
	inputArg* args = malloc(k*sizeof(inputArg));
	check_error(tids != NULL, "malloc failed args");
	
	for(int i=0; i < k; i++){
		args[i].threadNo = i + 1; //tredovi se broje pocevsi od 1, ne od 0
		check_errorThread(pthread_create(&tids[i], NULL, threadFunc, &args[i]), "pthread_create failed");
	}
	
	float maks = 0;
	int imaks;
	for(int i=0; i<k; i++){
		outputArg* retVal = NULL;
		check_errorThread(pthread_join(tids[i], (void**)&retVal), "pthread_join failed");
		if(retVal->max > maks){
			maks = retVal->max;
			imaks = i;
		}
		free(retVal);
	}
	
	printf("%d %f\n",imaks, maks);
	
	free(tids);
	free(args);
	free(argVektori);
	
	exit(EXIT_SUCCESS);
}
