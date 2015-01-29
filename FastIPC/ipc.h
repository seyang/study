/*
 *
 * Fast IPC LINUX Version
 *
 * Original Source Code Homepage
 *  : http://www.codeproject.com/Articles/14740/Fast-IPC-Communication-Using-Shared-Memory-and-Int
 *
 */

#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>

// Definitions
#define IPC_BLOCK_COUNT			512
#define IPC_BLOCK_SIZE			4096

#define IPC_MAX_ADDR			256

#define INFINITE LONG_MAX


// ---------------------------------------
// -- Inter-Process Communication class --
// ---------------------------------------------------------------
// Provides intercommunication between processes and there threads
// ---------------------------------------------------------------
class osIPC
{
public:
	// Block that represents a piece of data to transmit between the
	// client and server
	struct Block
	{
		// Variables
		long					Next;						// Next block in the circular linked list
		long					Prev;						// Previous block in the circular linked list

		volatile long			doneRead;					// Flag used to signal that this block has been read
		volatile long			doneWrite;					// Flag used to signal that this block has been written
		
		long					Amount;						// Amount of data help in this block
		//long 					_Padding;					// Padded used to ensure 64bit boundary

		char					Data[IPC_BLOCK_SIZE];		// Data contained in this block
	};

private:
	// Shared memory buffer that contains everything required to transmit
	// data between the client and server
	struct MemBuff
	{
		// Block data, this is placed first to remove the offset (optimisation)
		Block					m_Blocks[IPC_BLOCK_COUNT];	// Array of buffers that are used in the communication

		// Cursors
		volatile long			m_ReadEnd;					// End of the read cursor
		volatile long			m_ReadStart;				// Start of read cursor

		volatile long			m_WriteEnd;					// Pointer to the first write cursor, i.e. where we are currently writting to
		volatile long			m_WriteStart;				// Pointer in the list where we are currently writting

		// Signal
		sem_t					m_SemSignal;				// Semaphore Signal
		sem_t					m_SemAvail;					// Semaphore Avail
	};

public:
	// Get class
	class Get
	{
	public:
		// Construct / Destruct
		Get(const char* shmName);
		~Get();

	private:
		// Internal variables
		int						m_iFd;			// Shared memory file descriptor
		char					*m_sAddr;		// Address of this server
		MemBuff					*m_pBuf;		// Buffer that points to the shared memory
		struct timespec			m_tsTimeOut;	// Semaphore Timeout

	public:
		// Exposed functions
		long					read(void *pBuff, long buffSize, long timeout = INFINITE);
		char*					getAddress(void) { return m_sAddr; };

		// Block functions
		Block*					getBlock(long lTimeout = INFINITE);
		void					retBlock(Block* pBlock);

		// Create and destroy functions
		int						createIPC(const char* shmName);
		void					closeIPC(void);

		// Print Position Info
		void					printPos(void);
	};

	// Put class
	class Put
	{
	public:
		// Construct / Destruct
		Put(const char* shmName);
		~Put();

	private:
		// Internal variables
		int						m_iFd;			// Shared memory file descriptor
		char					*m_sAddr;		// Address of this server
		MemBuff					*m_pBuf;		// Buffer that points to the shared memory
		struct timespec			m_tsTimeOut;	// Semaphore Timeout

	public:
		// Exposed functions
		long					write(void *pBuff, long amount, long lTimeout = INFINITE);	// Writes to the buffer

		Block*					getBlock(long lTimeout = INFINITE);						// Gets a block
		void					postBlock(Block *pBlock);									// Posts a block to be processed				

		// Create and destroy functions
		int						createIPC(const char* shmName);
		void					closeIPC(void);

		// Print Position Info
		void					printPos(void);
	};
};

#endif
