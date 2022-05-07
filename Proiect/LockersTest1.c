#include <LockersLib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;

//pthread_mutex_t mtx;
myMutex mutex;

int decrease_count(int count)
{
	if (available_resources < count)
		return -1;
	else {
		//pthread_mutex_lock(&mtx);
		mutLock(&mutex);
		available_resources -= count;
		//pthread_mutex_unlock(&mtx);
		mutUnlock(&mutex);
	}
	return 0;
}

int increase_count(int count)
{
	//pthread_mutex_lock(&mtx);
	mutLock(&mutex);
	available_resources += count;
	//pthread_mutex_unlock(&mtx);
	mutUnlock(&mutex);
	return 0;
}

void *f_thr(void *arg)
{
	int res = *((int*)arg);
	int ok = decrease_count(res);
	if (ok == 0) {
		printf("Got %d resources %d remaining\n", res, available_resources);
		increase_count(res);
		printf("Released %d resources %d remaining\n", res, available_resources);
	}
	else
		printf("Not enough resources\n");
	return NULL;
}

int main()
{
	pthread_t thr[MAX_RESOURCES];
	/*if (pthread_mutex_init(&mtx, NULL)) {
		perror("Eroare la creare mutex");
		return errno;
	}*/
	mutInit(&mutex);
	printf("MAX_RESOURCES=%d\n", MAX_RESOURCES);
	for (int i = 0; i < MAX_RESOURCES; i++) {
		int *res = (int*)malloc(sizeof(int));
		(*res) = i + 1;
		if (pthread_create(&thr[i], NULL, f_thr, res)) {
			perror("Eroare la create");
			return errno;
		}
	}
	void *result;
	for (int i = 0; i < MAX_RESOURCES; i++)
		if (pthread_join(thr[i], &result)) {
			perror("Eroare la join");
			return errno;
		}
	//pthread_mutex_destroy(&mtx);
	mutDestroy(&mutex);
	return 0;
}


