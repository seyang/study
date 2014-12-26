#ifndef __CXX_PTHREADLOCK_H__
#define __CXX_PTHREADLOCK_H__

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

class PThreadLock {
	//protected:
	private:
		pthread_mutex_t *tmpMutex;
		pthread_mutexattr_t *tmpAttr;
		pthread_mutex_t chkMtx;
		bool setRobust;
		bool setShm;
		bool setComplete;
		int tmpRet;
	public:
		PThreadLock(pthread_mutex_t *_mtx, pthread_mutexattr_t *_mtx_attr);
		int setAttrNormal();
		int setAttrErrChk();
		int setAttrShm();
		int setAttrRobust();
		int initMutex();
		int initOpen();
		void lock();
		void unlock();
};

class PTLockS : public PThreadLock {
	public:
		PTLockS(pthread_mutex_t *_mtx, pthread_mutexattr_t *_mtx_attr)
			: PThreadLock(_mtx, _mtx_attr) { }
		PTLockS(char *_shm_name, pthread_mutex_t *_mtx, pthread_mutexattr_t *_mtx_attr) {
				printf("PTLockS Constructor\n");
			}
		int initForShm();
	
};

#endif
