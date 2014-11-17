#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <PThreadLock.h>

typedef struct shm_struct {
	pthread_mutex_t  mtx;
	pthread_mutexattr_t  mtx_attr;
	//pthread_cond_t   cond;
	int i, j;
} shm_struct_t;
