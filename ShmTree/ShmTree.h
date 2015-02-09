#include <exception>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/file.h>

#define MAX_NODE_NAME	128	
#define MAX_TOK_CNT		64	
#define MAX_TOK_LEN		128
#define MAX_CHILD_CNT	128

typedef struct TreeNode {
	int				iTokCnt;
	char			cNodeName[MAX_TOK_CNT][MAX_NODE_NAME];
	long			lChildNodeIdx[MAX_CHILD_CNT];
} TREE_NODE;

typedef struct MemBuff {
	long			lLastOffset;
} MEM_BUF;

class ShmTree {
	private:
		int			m_iFd;
		MEM_BUF*	m_pBuf;
		char* 		m_pDataStart;

	public:
		ShmTree();
		~ShmTree();

	public:
		int			setShm(const char *shmName);
		int			addNode(TREE_NODE *_node);
		TREE_NODE*	makeNode(const char *_nodeName);
		void		printNode(TREE_NODE *_node);
};
