# Lockers-Project

Me and two colleagues made a C library, where we implemented our own version of lockers (mutex, semaphore and RW locks).

In this process we also learned some Linux commands, since we had to work with C from the linux terminal.

Run some exemples:
* gcc -o lockers1 LockersTest1.c LockersLib.c -I. -pthread
* gcc -o lockers2 LockersTest2.c LockersLib.c -I. -pthread
* gcc -o lockers3 LockersTest3.c LockersLib.c -I. -pthread
