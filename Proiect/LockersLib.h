typedef struct {
	int val;
} myMutex;

void mutLock(myMutex *);
void mutUnlock(myMutex *);
void mutInit(myMutex *);
void mutDestroy(myMutex *);

typedef struct {
	int val;
} mySemaphore;

void semInit(mySemaphore *, int);
void semWait(mySemaphore *);
void semPost(mySemaphore *);
void semDestroy(mySemaphore *);

typedef struct {
	int val;
	myMutex mut1;
	myMutex mut2;
} myRWLock;

void rwInit(myRWLock *);
void rwReadLock(myRWLock *);
void rwReadUnlock(myRWLock *);
void rwWriteLock(myRWLock *);
void rwWriteUnlock(myRWLock *);
