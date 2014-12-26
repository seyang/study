#include "ShmQueue.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s [IPC_ID]\n", argv[0]);
		exit(1);
	}

	ShmQueue q1(atoi(argv[1]));
	q1.ShmQueueOpen();

	char msg[1024];
	
	while(1) {
		q1.GetQueue(msg);
		//sleep(1);
	}

	return 0;
}
