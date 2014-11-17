#include <PThreadLock.h>

PThreadLock::PThreadLock(pthread_mutex_t *_mtx, pthread_mutexattr_t *_mtx_attr) {
	if (_mtx == NULL || _mtx_attr == NULL) {
		printf("Null pointer\n");
		return;
	}

	tmpMutex = _mtx;
	tmpAttr = _mtx_attr;

	memset(&chkMtx, 0x00, sizeof(pthread_mutex_t));
	setComplete = false;
}

int PThreadLock::getAttrInfo() {
	int chkAttr;

	// Check if mutex is set
	if (memcmp(tmpMutex, &chkMtx, sizeof(pthread_mutex_t)) == 0) {
		printf("[ERR] Mutex must be itialized\n");
		return -1;
	}

	// Check & Set shared
	if ((tmpRet = pthread_mutexattr_getpshared(tmpAttr, &chkAttr))) {
		printf("Error: pthread_mutexattr_getpshared()\n");
		return -1;
	}
	if (chkAttr == PTHREAD_PROCESS_SHARED) {
		setShm = true;
	} else {
		setShm = false;
	}
	printf("setShm in shm: %s\n", setShm?"true":"false");

	// Check & Set robust
	if ((tmpRet = pthread_mutexattr_getrobust(tmpAttr, &chkAttr))) {
		printf("Error: pthread_mutexattr_getrobust()\n");
		return -1;
	}

	if (chkAttr == PTHREAD_MUTEX_ROBUST) {
		setRobust = true;
	} else {
		setRobust = false;
	}
	printf("setRobust in shm: %s\n", setRobust?"true":"false");

	setComplete = true;

	return 0;
}

int PThreadLock::setAttrNormal() {
	if ((tmpRet = pthread_mutexattr_settype(tmpAttr, PTHREAD_MUTEX_NORMAL))) {
		printf("Error: pthread_mutexattr_settype()\n");
		return tmpRet;
	}
	return 0;
}

int PThreadLock::setAttrErrChk() {
	if ((tmpRet = pthread_mutexattr_settype(tmpAttr, PTHREAD_MUTEX_ERRORCHECK))) {
		printf("Error: pthread_mutexattr_settype()\n");
		return tmpRet;
	}
	return 0;
}

int PThreadLock::setAttrShm() {
	if (setShm == true) {
		return 0;
	}
	if ((tmpRet = pthread_mutexattr_setpshared(tmpAttr, PTHREAD_PROCESS_SHARED))) {
		printf("Error: pthread_mutexattr_setpshared()\n");
		return tmpRet;
	}
	setShm = true;
	printf("Set setShm to true\n");
	return 0;
}

int PThreadLock::setAttrRobust() {
	if (setRobust == true) {
		return 0;
	}
	if ((tmpRet = pthread_mutexattr_setrobust(tmpAttr, PTHREAD_MUTEX_ROBUST))) {
		printf("Error: pthread_mutexattr_setrobust()\n");
		return tmpRet;
	}
	setRobust = true;
	printf("Set setRobust to true\n");
	return 0;
}

int PThreadLock::setMutexInit() {
	int ret;
	pthread_mutex_t chkMtx;
	memset(&chkMtx, 0x00, sizeof(pthread_mutex_t));
	if (memcmp(tmpMutex, &chkMtx, sizeof(pthread_mutex_t)) != 0) {
		printf("[ERR] Mutex is already itialized\n");
		return -1;
	}

	if ((ret = pthread_mutex_init(tmpMutex, tmpAttr)) != 0) {
		printf("[ERR] pthread_mutex_init(strerr=\"%s\")\n" , strerror(ret));
		return ret;
	}

	setComplete = true;

	return 0;
}

void PThreadLock::lock() {
	int ret;
	if (!setComplete) {
		return;
	}
	if ((ret = pthread_mutex_lock(tmpMutex)) != 0) {
		if ((setRobust == true) && (ret == EOWNERDEAD)) {
			//printf("EOWNERDEAD detected\n");
			pthread_mutex_consistent(tmpMutex);
		} else {
			printf("[ERR] pthread_mutex_lock(strerr=\"%s\")\n" , strerror(ret));
		}
	}
}

void PThreadLock::unlock() {
	if (!setComplete) {
		return;
	}
	pthread_mutex_unlock(tmpMutex);
}


int PTSLock::initForShared() {
	int ret;
	if ((ret = setAttrNormal()) != 0) {
		return ret;
	}
	if ((ret = setAttrShm()) != 0) {
		return ret;
	}
	if ((ret = setAttrRobust()) != 0) {
		return ret;
	}
	if ((ret = setMutexInit()) != 0) {
		return ret;
	}
	return 0;
}
