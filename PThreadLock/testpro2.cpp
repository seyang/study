#include "testpro.h"

shm_struct_t *g_shm;
pthread_mutexattr_t mtx_attr;
pthread_condattr_t cond_attr;
PTSLock *sharedLock;

int shm_init() {
	int fd;
	int ret = 0;  
	int shm_size;

	if ((fd = shm_open("/testpro", O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IROTH)) == -1) {
		if (errno == EEXIST) {
			if ((fd = shm_open("/testpro", O_RDWR, 0)) == -1) {
				perror("shm_open");
				return -1;
			}
			g_shm = (shm_struct_t *)mmap(NULL, sizeof(shm_struct_t), PROT_READ|PROT_WRITE
					, MAP_SHARED, fd, 0);
			if (g_shm == MAP_FAILED) {
				perror("mmap");
				return -1;
			}
			sharedLock = new PTSLock(&g_shm->mtx, &g_shm->mtx_attr);
			if (sharedLock->getAttrInfo() == -1) {
				return -1;
			}
			close(fd);
			return ret;
		}
	}
	if (fd == -1) {
		fprintf(stderr, "shm_open() error\n");
		return fd;
	}
	shm_size = sizeof(shm_struct_t);
	ret = ftruncate(fd, shm_size);
	if (ret == -1) {
		fprintf(stderr, "ftruncate() failed. errno=%d\n", errno);
		return ret;
	}
	g_shm = (shm_struct_t *)mmap(NULL, shm_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (g_shm == MAP_FAILED) {
		fprintf(stderr, "mmap() failed\n");
		return -1;
	}
	close(fd);

	memset(&g_shm->mtx, 0x00, sizeof(pthread_mutex_t));
	memset(&g_shm->mtx_attr, 0x00, sizeof(pthread_mutexattr_t));
	g_shm->i = 0;
	g_shm->j = 0;

	sharedLock = new PTSLock(&g_shm->mtx, &g_shm->mtx_attr);
	sharedLock->initForShared();

	return ret;
}

int main() {
	int ret = 0; 

	ret = shm_init();

	if (ret) {
		fprintf(stderr, "shm_init() failed. \n");
		return ret;
	}
	
	sharedLock->lock();
	for (int i=0; i<10; i++) {
		g_shm->i++;   
		fprintf(stdout, "i = %d, j = %d\n", g_shm->i, g_shm->j);
		sleep(1);
	}
	sharedLock->unlock();

	printf("i: %d\n", g_shm->i);

	return ret;
}
