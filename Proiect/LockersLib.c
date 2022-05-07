#include <stdio.h>
#include <stdatomic.h>
#include <LockersLib.h>

// MUTEX
void mutInit(myMutex *mutex) {
	atomic_init(&mutex->val, 0);
}

void mutLock(myMutex *mutex) {
	while(__sync_val_compare_and_swap(&mutex->val, 0, 1) != 0) { }
}

void mutUnlock(myMutex *mutex) {
	atomic_store(&mutex->val, 0);
}

void mutDestroy(myMutex *mutex) {
	atomic_store(&mutex->val, -1);
}

// SEMAFOR
void semInit(mySemaphore *sem, int x) {
	atomic_init(&sem->val, x);
}

void semWait(mySemaphore *sem) {
	while(atomic_load(&sem->val) == 0) { }
	atomic_fetch_sub(&sem->val, 1);
}

void semPost(mySemaphore *sem) {
	atomic_fetch_add(&sem->val, 1);
}

void semDestroy(mySemaphore *sem) {
	atomic_store(&sem->val, -1);
}

// RWLOCK
void rwInit(myRWLock *rw) {
	atomic_init(&rw->val, 0);
	mutInit(&rw->mut1);
	mutInit(&rw->mut2);
}
void rwReadLock(myRWLock *rw) {
	mutLock(&rw->mut1);
	atomic_fetch_add(&rw->val, 1);
	if (atomic_load(&rw->val) == 1)
		mutLock(&rw->mut2);
	mutUnlock(&rw->mut1);
}
void rwReadUnlock(myRWLock *rw) {
	mutLock(&rw->mut1);
	atomic_fetch_sub(&rw->val, 1);
	if (atomic_load(&rw->val) == 0)
		mutUnlock(&rw->mut2);
	mutUnlock(&rw->mut1);
}
void rwWriteLock(myRWLock *rw) {
	mutLock(&rw->mut2);
}
void rwWriteUnlock(myRWLock *rw) {
	mutUnlock(&rw->mut2);
}
