#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <LockersLib.h>

#define N 5

myRWLock rw;

void *tfunR(void *v)
{
	int *tid = (int *)v;
	printf("Read %d reached the barrier\n", *tid);
	rwReadLock(&rw);
	printf("Read %d passed the barrier\n", *tid);
	rwReadUnlock(&rw);
	free(tid);
	return NULL;
} 

void *tfunW(void *v)
{
	int *tid = (int *)v;
	printf("Write %d reached the barrier\n", *tid);
	rwWriteLock(&rw);
	printf("Write %d passed the barrier\n", *tid);
	rwWriteUnlock(&rw);
	free(tid);
	return NULL;
} 

int main()
{	
	pthread_t thrR[N];
	pthread_t thrW[N];
	rwInit(&rw);
	printf("NTHRS_READ=%d\n", N);
	printf("NTHRS_WRITE=%d\n", N);
	for (int i = 0; i < N; i++) {
		int *idR = (int*)malloc(sizeof(int));
		int *idW = (int*)malloc(sizeof(int));
		*idR = i;
		*idW = i;
		if (pthread_create(&thrR[i], NULL, tfunR, idR)) {
			perror("Eroare la create");
			return errno;
		}
		if (pthread_create(&thrW[i], NULL, tfunW, idW)) {
			perror("Eroare la create");
			return errno;
		}
	}
	void *resultR;
	void *resultW;
	for (int i = 0; i < N; i++) {
		if (pthread_join(thrR[i], &resultR)) {
			perror("Eroare la join");
			return errno;
		}
		if (pthread_join(thrW[i], &resultW)) {
			perror("Eroare la join");
			return errno;
		}
	}
	return 0;
}
