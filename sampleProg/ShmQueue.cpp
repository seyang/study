#include "ShmQueue.h"

ShmQueueData::ShmQueueData() {
	front = rear = 0;

	qSize = (SHM_SIZE/DATA_SIZE)-1;
}

ShmQueue::ShmQueue(int _ipcKey) {
	ipcKey = (key_t)_ipcKey;
}

/*
 * SharedMemory
 */

int ShmQueue::CreateShm() {
	ShmQueueData _tempData;

	shmid = shmget(ipcKey, SHM_SIZE, IPC_CREAT|IPC_EXCL|0644);
	if(shmid == -1) {
		if(errno == EEXIST) {
			if ((shmid = shmget(ipcKey, 0, 0)) == -1) {
                perror("shmget(open)");
                return -1;
            }
			myQ = (ShmQueueData *)shmat(shmid, NULL, 0);
			printf("[INFO] Shared memory is opened: key(%d), id(%d)\n", ipcKey, shmid);
		} else {
			perror("shmget");
			return -1;
		}
	} else {
		myQ = (ShmQueueData *)shmat(shmid, NULL, 0);
		memcpy(myQ, &_tempData, sizeof(ShmQueueData));
		
		printf("[INFO] Shared memory is created: %d, %d\n", ipcKey, shmid);
	}

	return 0;
}

int ShmQueue::ResetIPC() {
	if (CreateShm() == -1) {
		return -1;
	}

	if (InitSem(ipcKey, &semid1) == -1 || InitSem(ipcKey+1, &semid2) == -1) {
		return -1;
	}
	if(shmctl(shmid, IPC_RMID, (struct shmid_ds *)NULL) == -1) {
		perror("shmctl");
		return -1;
	} else {
		printf("[INFO] Shared memory is deleted: %d\n", shmid);
	}

	if(semctl(semid1, 0, IPC_RMID) == -1) {
		perror("semctl");
		return -1;
	} else {
		printf("[INFO] Semaphore1 is deleted: %d\n", semid1);
	}

	if(semctl(semid2, 0, IPC_RMID) == -1) {
		perror("semctl");
		return -1;
	} else {
		printf("[INFO] Semaphore2 is deleted: %d\n", semid2);
	}
	return 0;
}

int ShmQueue::ShmQueueOpen() {
	if (CreateShm() == -1) {
		return -1;
	}

	if (InitSem(ipcKey, &semid1) == -1 || InitSem(ipcKey+1, &semid2) == -1) {
		return -1;
	}

	return 0;
}

/*
 * Semaphore
 */

int ShmQueue::InitSem(int _ipcKey, int *_semid) {
	union semun semunarg;
	int status = 0;

	*_semid = semget(_ipcKey, 1, IPC_CREAT | IPC_EXCL | 0600);
	if (*_semid == -1) {
		if (errno == EEXIST) {
			*_semid = semget(_ipcKey, 1, 0);
			printf("[INFO] Semaphore is opened: key(%d), id(%d)\n", _ipcKey, *_semid);
		}
	}
	else {
		semunarg.val = 0;
		status = semctl(*_semid, 0, SETVAL, semunarg);
        printf("[INFO] Semaphore is created: key(%d), id(%d)\n", _ipcKey, *_semid);
	}

	if (*_semid == -1) {
		perror("initsem");
		return -1;
	}

    if (status == -1) {
        perror("sem status");
        return -1;
    }

	return *_semid;
}

int ShmQueue::semlock(int semid) {
	struct sembuf buf;

	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	//printf("[INFO] semlock: %d\n", semid);
	if (semop(semid, &buf, 1) == -1) {
        PrintSem();
        //printf("semid: %d, ", semid);
		perror("semlock failed");
		return -1;
	}
	return 0;
}

int ShmQueue::semunlock(int semid) {
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    //printf("[INFO] semunlock: %d\n", semid);
    if (semop(semid, &buf, 1) == -1) {
        perror("semunlock failed");
        return -1;
    }

	return 0;
}

int ShmQueue::PrintSem() {
    printf("semaphore1: %d, %d\n", semid1, semctl(semid1, 0, GETVAL));
    printf("semaphore2: %d, %d\n", semid2, semctl(semid2, 0, GETVAL));


    return 0;
}

/*
 * Circular Queue
 */

int ShmQueue::PutQueue(const char *msg) {
	if ((myQ->rear + 1) % myQ->qSize == myQ->front) {
        printf("[ERR] Queue is full\n");
		semlock(semid1);
	}

	strncpy(myQ->shmQ[myQ->rear].data, msg, DATA_SIZE);
	myQ->rear = ++myQ->rear % myQ->qSize;

    
    if ((myQ->front + 1) % myQ->qSize == myQ->rear) {
        semunlock(semid2);
    }

	return 0;
}

int ShmQueue::GetQueue(char *msg) {

	if (myQ->front == myQ->rear) {
        printf("[ERR] Queue is empty\n");
        semlock(semid2);
	}

	strncpy(msg, myQ->shmQ[myQ->front].data, DATA_SIZE);

	myQ->front = ++myQ->front % myQ->qSize;

    printf("[front: %03d][rear: %03d] [<<] Get: %s\n", myQ->front, myQ->rear, msg);

    if ((myQ->rear + 2) % myQ->qSize == myQ->front) {
        semunlock(semid1);
    }

    if (myQ->front == myQ->rear) {
        printf("[ERR] Queue is empty2\n");
        semlock(semid2);
    }

	return 0;
}

int ShmQueue::PrintQueue() {
	int i, end;
	if (myQ->front > myQ->rear) {
		end = myQ->rear + myQ->qSize;
	} else {
		end = myQ->rear;
	}
	printf("-----------------------------------------\n");
	printf("[front: %d][rear: %d]\n", myQ->front, myQ->rear);
	for(i = myQ->front; i<end; i++) {
		printf("[Data(%d) %s]\n", i, myQ->shmQ[i%myQ->qSize].data);
	}
	printf("-----------------------------------------\n");
	return 0;
}

void ShmQueue::test() {
	printf("data size:%ld\n", sizeof(ShmQueueData));
}


//int main(int argc, char *argv[]) {
	//ShmQueue q1;
	//q1.ShmQueueOpen("shmfile");

	////q1.test();
	//char msg[128];
	
	////while(1) {

	////}
	//q1.PutQueue("Test message 1");
	//q1.GetQueue(msg);
	//q1.PrintQueue();

	//return 0;
//}
