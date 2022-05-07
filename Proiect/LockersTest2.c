#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <LockersLib.h>

#define N 5

int nr = 0;
//pthread_mutex_t mtx;
myMutex mtx;
//sem_t sem;
mySemaphore sem;

void barrier_point()
{
	//pthread_mutex_lock(&mtx);
	mutLock(&mtx);
	nr++;
	//pthread_mutex_unlock(&mtx);
	mutUnlock(&mtx);

	if (nr == N) {
		for (int i = 0; i < N; i++)
			/*if (sem_post(&sem)) {
				perror(NULL);
				return NULL;
			}*/
			semPost(&sem);
	}
	else {
		/*if (sem_wait(&sem)) {
			perror(NULL);
			return NULL;
		}*/
		semWait(&sem);
	}
}

void *tfun(void *v)
{
	int *tid = (int *)v;
	printf("%d reached the barrier\n", *tid);
	barrier_point();
	printf("%d passed the barrier\n", *tid);
	free(tid);
	return NULL;
} 

int main()
{	
	pthread_t thr[N];
	/*if (pthread_mutex_init(&mtx, NULL)) {
		perror("Eroare la creare mutex");
		return errno;
	}*/
	mutInit(&mtx);
	/*if (sem_init(&sem, 0, 0)) {
		perror("Eroare la creare semafor");
		return errno;
	}*/
	semInit(&sem, 0);
	printf("NTHRS=%d\n", N);
	for (int i = 0; i < N; i++) {
		int *id = (int*)malloc(sizeof(int));
		*id = i;
		if (pthread_create(&thr[i], NULL, tfun, id)) {
			perror("Eroare la create");
			return errno;
		}
	}
	void *result;
	for (int i = 0; i < N; i++)
		if (pthread_join(thr[i], &result)) {
			perror("Eroare la join");
			return errno;
		}
	//pthread_mutex_destroy(&mtx);
	mutDestroy(&mtx);
	//sem_destroy(&sem);
	semDestroy(&sem);
	return 0;
}
