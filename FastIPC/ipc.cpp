#include "ipc.h"

#define CASVal(Dest, Comp, Exchange) __sync_val_compare_and_swap(Dest, Comp, Exchange)
#define min(a, b) (((a) < (b)) ? (a) : (b))

inline void getTimeOut(struct timespec *_tsRet, long _lTimeOut) {
	clock_gettime(CLOCK_MONOTONIC, _tsRet);
	_tsRet->tv_nsec += _lTimeOut*1000000;
	if (_tsRet->tv_nsec > 1000000000) {
		_tsRet->tv_sec += (_tsRet->tv_nsec)/1000000000;
		_tsRet->tv_nsec = (_tsRet->tv_nsec)%1000000000;
	}
}

osIPC::Get::Get(const char* shmName) {
	// Set default params
	m_pBuf = NULL;
	m_sAddr = NULL;

	// create the server
	createIPC(shmName);
};

osIPC::Get::~Get(void) {
	// Free memory
	if (m_sAddr) {
		free(m_sAddr);
		m_sAddr = NULL;
	}

	// Close the server
	closeIPC();
};

int osIPC::Get::createIPC(const char* shmName) {
	long shmSize = sizeof(struct MemBuff);

	// create shared memory
	if((m_iFd = shm_open(shmName, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1) {
		if(errno == EEXIST) {
			struct stat sb;

			if((m_iFd = shm_open(shmName, O_RDWR, 0)) == -1) {
				perror("[rIPC] <IPCGet> shm_open");
				return -1;
			}

			while (true) {
				if (fstat(m_iFd, &sb) == -1) {
					perror("fstat");
					return -1;
				}
				if (sb.st_size != 0) {
					break;
				}
			}

			flock(m_iFd, LOCK_EX);
			m_pBuf = (MemBuff *)mmap64(NULL, shmSize, PROT_READ|PROT_WRITE, MAP_SHARED, m_iFd, 0);
			if(m_pBuf == MAP_FAILED) {
				perror("[rIPC] <IPCGet> mmap64");
				return -1;
			}

#ifdef PRINT_SYSTEM_MSG
			printf("[rIPC] SHM is opened [name: %s] [size: %ld B]\n", shmName, size);
#endif

			flock(m_iFd, LOCK_UN);
		} else {
			perror("[rIPC] <IPCGet> shm_create");
			return -1;
		}
	} else {
		flock(m_iFd, LOCK_EX);

		if (ftruncate64(m_iFd, shmSize) == -1) {
			perror("[rIPC] <IPCGet> ftruncate64");
			return -1;
		}

		m_pBuf = (MemBuff *)mmap64(NULL, shmSize, PROT_READ|PROT_WRITE, MAP_SHARED, m_iFd, 0);
		if(m_pBuf == MAP_FAILED) {
			perror("[rIPC] <IPCGet> mmap64");
			return -1;
		}

#ifdef PRINT_SYSTEM_MSG
		printf("[rIPC] SHM is created [name: %s] [size: %ld B]\n", shmName, size);
#endif

		// Initialize semaphore
		if(sem_init(&(m_pBuf->m_SemSignal), 1, 0) < 0) {
			perror("[rIPC] <IPCPut> sem_init");
			return -1;
		}
		if(sem_init(&(m_pBuf->m_SemAvail), 1, 0) < 0) {
			perror("[rIPC] <IPCPut> sem_init");
			return -1;
		}

		memset(m_pBuf, 0x00, sizeof(struct MemBuff));

		int N = 1;
		m_pBuf->m_Blocks[0].Next = 1;
		m_pBuf->m_Blocks[0].Prev = (IPC_BLOCK_COUNT-1);
		for (;N < IPC_BLOCK_COUNT-1; N++)
		{
			// Add this block into the available list
			m_pBuf->m_Blocks[N].Next = (N+1);
			m_pBuf->m_Blocks[N].Prev = (N-1);
		}
		m_pBuf->m_Blocks[N].Next = 0;
		m_pBuf->m_Blocks[N].Prev = (IPC_BLOCK_COUNT-2);

		// Initialize the pointers
		m_pBuf->m_ReadEnd = 1;
		m_pBuf->m_ReadStart = 1;
		m_pBuf->m_WriteEnd = 1;
		m_pBuf->m_WriteStart = 1;

		flock(m_iFd, LOCK_UN);
	}

	return 0;
};

void osIPC::Get::closeIPC(void) {
	m_pBuf = NULL;
	free(m_pBuf);
	close(m_iFd);
};

osIPC::Block* osIPC::Get::getBlock(long lTimeout) {
	// Grab another block to read from
	// Enter a continous loop (this is to make sure the operation is atomic)
	for (;;)
	{
		// Check if there is room to expand the read start cursor
		long blockIndex = m_pBuf->m_ReadStart;
		Block *pBlock = m_pBuf->m_Blocks + blockIndex;
		if (blockIndex == m_pBuf->m_WriteEnd) {
			// No room is available, wait for room to become available
			if (lTimeout == INFINITE) {
				sem_wait(&(m_pBuf->m_SemSignal));
			} else {
				getTimeOut(&m_tsTimeOut, lTimeout);
				if(sem_timedwait(&(m_pBuf->m_SemSignal), &m_tsTimeOut) < 0) {
					if(errno == ETIMEDOUT) {
						// Timeout
						return NULL;
					}
				}
			}
			continue;
		}

		// Make sure the operation is atomic
		if (CASVal(&m_pBuf->m_ReadStart, blockIndex, pBlock->Next) == blockIndex) {
			return pBlock;
		}

		// The operation was interrupted by another thread.
		// The other thread has already stolen this block, try again
		continue;
	}
};

void osIPC::Get::retBlock(osIPC::Block* pBlock) {
	// Set the done flag for this block
	pBlock->doneRead = 1;

	// Move the read pointer as far forward as we can
	for (;;) {
		// Try and get the right to move the poitner
		long blockIndex = m_pBuf->m_ReadEnd;
		pBlock = m_pBuf->m_Blocks + blockIndex;
		if (CASVal(&pBlock->doneRead, 1, 0) != 1) {
			// If we get here then another thread has already moved the pointer
			// for us or we have reached as far as we can possible move the pointer
			return;
		}

		// Move the pointer one forward (interlock protected)
		CASVal(&m_pBuf->m_ReadEnd, blockIndex, pBlock->Next);
		
		// Signal availability of more data but only if a thread is waiting
		if (pBlock->Prev == m_pBuf->m_WriteStart) {
			sem_post(&(m_pBuf->m_SemAvail));
		}
	}
};

long osIPC::Get::read(void *pBuff, long buffSize, long lTimeout) {
	// Grab a block
	Block *pBlock = getBlock(lTimeout);
	if (!pBlock) return 0;

	// Copy the data
	long lAmount = min(pBlock->Amount, buffSize);
	memcpy(pBuff, pBlock->Data, lAmount);

	// Return the block
	retBlock(pBlock);

	// Success
	return lAmount;
};

void osIPC::Get::printPos() {
	printf("WS(%ld), WE(%ld), RS(%ld), RE(%ld)\n"
			, m_pBuf->m_WriteStart, m_pBuf->m_WriteEnd
			, m_pBuf->m_ReadStart, m_pBuf->m_ReadEnd);
}

osIPC::Put::Put(const char* shmName) {
	// Set default params
	m_pBuf = NULL;
	m_sAddr = NULL;

	// create the server
	createIPC(shmName);
};

int osIPC::Put::createIPC(const char* shmName) {
	long shmSize = sizeof(struct MemBuff);

	// create shared memory
	if((m_iFd = shm_open(shmName, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1) {
		if(errno == EEXIST) {
			struct stat sb;

			if((m_iFd = shm_open(shmName, O_RDWR, 0)) == -1) {
				perror("[rIPC] <IPCGet> shm_open");
				return -1;
			}

			while (true) {
				if (fstat(m_iFd, &sb) == -1) {
					perror("fstat");
					return -1;
				}
				if (sb.st_size != 0) {
					break;
				}
			}

			flock(m_iFd, LOCK_EX);
			m_pBuf = (MemBuff *)mmap64(NULL, shmSize, PROT_READ|PROT_WRITE, MAP_SHARED, m_iFd, 0);
			if(m_pBuf == MAP_FAILED) {
				perror("[rIPC] <IPCGet> mmap64");
				return -1;
			}

#ifdef PRINT_SYSTEM_MSG
			printf("[rIPC] SHM is opened [name: %s] [size: %ld B]\n", shmName, size);
#endif

			flock(m_iFd, LOCK_UN);
		} else {
			perror("[rIPC] <IPCGet> shm_create");
			return -1;
		}
	} else {
		flock(m_iFd, LOCK_EX);

		if (ftruncate64(m_iFd, shmSize) == -1) {
			perror("[rIPC] <IPCGet> ftruncate64");
			return -1;
		}

		m_pBuf = (MemBuff *)mmap64(NULL, shmSize, PROT_READ|PROT_WRITE, MAP_SHARED, m_iFd, 0);
		if(m_pBuf == MAP_FAILED) {
			perror("[rIPC] <IPCGet> mmap64");
			return -1;
		}

#ifdef PRINT_SYSTEM_MSG
		printf("[rIPC] SHM is created [name: %s] [size: %ld B]\n", shmName, size);
#endif

		// Initialize semaphore
		if(sem_init(&(m_pBuf->m_SemSignal), 1, 0) < 0) {
			perror("[rIPC] <IPCPut> sem_init");
			return -1;
		}
		if(sem_init(&(m_pBuf->m_SemAvail), 1, 0) < 0) {
			perror("[rIPC] <IPCPut> sem_init");
			return -1;
		}

		memset(m_pBuf, 0x00, sizeof(struct MemBuff));

		int N = 1;
		m_pBuf->m_Blocks[0].Next = 1;
		m_pBuf->m_Blocks[0].Prev = (IPC_BLOCK_COUNT-1);
		for (;N < IPC_BLOCK_COUNT-1; N++)
		{
			// Add this block into the available list
			m_pBuf->m_Blocks[N].Next = (N+1);
			m_pBuf->m_Blocks[N].Prev = (N-1);
		}
		m_pBuf->m_Blocks[N].Next = 0;
		m_pBuf->m_Blocks[N].Prev = (IPC_BLOCK_COUNT-2);

		// Initialize the pointers
		m_pBuf->m_ReadEnd = 1;
		m_pBuf->m_ReadStart = 1;
		m_pBuf->m_WriteEnd = 1;
		m_pBuf->m_WriteStart = 1;

		flock(m_iFd, LOCK_UN);
	}

	return 0;
};

void osIPC::Put::closeIPC(void) {
	m_pBuf = NULL;
	free(m_pBuf);
	close(m_iFd);
};

osIPC::Put::~Put(void) {
	// Free memory
	if (m_sAddr) {
		free(m_sAddr);
		m_sAddr = NULL;
	}

	// Close the server
	closeIPC();
};

osIPC::Block* osIPC::Put::getBlock(long lTimeout)
{
	// Grab another block to write too
	// Enter a continous loop (this is to make sure the operation is atomic)
	for (;;)
	{
		// Check if there is room to expand the write start cursor
		long blockIndex = m_pBuf->m_WriteStart;
		Block *pBlock = m_pBuf->m_Blocks + blockIndex;
		if (pBlock->Next == m_pBuf->m_ReadEnd) {
			// No room is available, wait for room to become available
			if (lTimeout == INFINITE) {
				sem_wait(&(m_pBuf->m_SemAvail));
			} else {
				getTimeOut(&m_tsTimeOut, lTimeout);
				if(sem_timedwait(&(m_pBuf->m_SemAvail), &m_tsTimeOut) < 0)
					if(errno == ETIMEDOUT) {
						// Timeout
						return NULL;
					}
			}
			continue;
		}

		// Make sure the operation is atomic
		if (CASVal(&m_pBuf->m_WriteStart, blockIndex, pBlock->Next) == blockIndex) {
			return pBlock;
		}

		// The operation was interrupted by another thread.
		// The other thread has already stolen this block, try again
		continue;
	}
};

void osIPC::Put::postBlock(Block *pBlock)
{
	// Set the done flag for this block
	pBlock->doneWrite = 1;

	// Move the write pointer as far forward as we can
	for (;;)
	{
		// Try and get the right to move the poitner
		long blockIndex = m_pBuf->m_WriteEnd;
		pBlock = m_pBuf->m_Blocks + blockIndex;
		if (CASVal(&pBlock->doneWrite, 1, 0) != 1) {
			// If we get here then another thread has already moved the pointer
			// for us or we have reached as far as we can possible move the pointer
			return;
		}

		// Move the pointer one forward (interlock protected)
		CASVal(&m_pBuf->m_WriteEnd, blockIndex, pBlock->Next);
		
		// Signal availability of more data but only if threads are waiting
		if (blockIndex == m_pBuf->m_ReadStart) {
			sem_post(&(m_pBuf->m_SemSignal));
		}
	}
};

long osIPC::Put::write(void *pBuff, long amount, long lTimeout)
{
	// Grab a block
	Block *pBlock = getBlock(lTimeout);
	if (!pBlock) return 0;

	// Copy the data
	long lAmount = min(amount, IPC_BLOCK_SIZE);
	memcpy(pBlock->Data, pBuff, lAmount);
	pBlock->Amount = lAmount;

	// Post the block
	postBlock(pBlock);

	// Fail
	return 0;
};

void osIPC::Put::printPos() {
	printf("WS(%ld), WE(%ld), RS(%ld), RE(%ld)\n"
			, m_pBuf->m_WriteStart, m_pBuf->m_WriteEnd
			, m_pBuf->m_ReadStart, m_pBuf->m_ReadEnd);
}
