#include "ShmQueue.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s [IPC_ID]\n", argv[0]);
		exit(1);
	}

	ShmQueue q1(atoi(argv[1]));
	q1.ShmQueueOpen();

	char msg[1024] = "Test Message";
    int cnt = 0;
    //int i = 0;
	
    //while(1) {
    for (int i=0; i<100000; i++) {
        snprintf(msg, 1024, "Message %d", cnt++);
		q1.PutQueue(msg);
        //sleep(1);
	}

    printf("put end\n");
    //while(1) {
        //q1.PrintSem();
        //sleep(1);
    //}
	//q1.GetQueue(msg);
	//q1.PrintQueue();

	return 0;
}
