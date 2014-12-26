#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SHM_SIZE 1024*1024
#define DATA_SIZE 1024 
#define Q_CNT 1024

using namespace std;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

typedef struct QueueData {
	long status;
	char data[DATA_SIZE];
} QData;

typedef ShmQueueData {
	int front;
	int rear;
	unsigned long  qSize;
	QData shmQ[Q_CNT];
} ShmQStruct;

class ShmQueue {
	private:
		int shmid;
		int semid1;
		int semid2;
		key_t ipcKey;
		ShmQueueData *myQ;
	public:
		ShmQueue(int _ipcKey);
		// shm
		int CreateShm();
		// queue
		int PutQueue(const char *msg);
		int GetQueue(char *msg);
		int PrintQueue();
		// sem
		int InitSem(int _ipcKey, int *_semid);
		int semlock(int semid);
		int semunlock(int semid);
        int PrintSem();
		// shm queue
		int ShmQueueOpen();
        // etc
        int ResetIPC();
		void test();
		
};
