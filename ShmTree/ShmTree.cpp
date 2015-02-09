#include "ShmTree.h"

ShmTree::ShmTree() {
	setShm("TestShm");

	TREE_NODE node;
	snprintf(node.cNodeName[0], MAX_TOK_LEN, "ROOT");
	node.iTokCnt = 1;
	memset(node.lChildNodeIdx, 0x00, sizeof(long)*MAX_CHILD_CNT);

	memcpy(m_pBuf+sizeof(long), &node, sizeof(TREE_NODE));
	m_pDataStart = (char *)m_pBuf + sizeof(long);
}

ShmTree::~ShmTree() {
}

int ShmTree::setShm(const char* shmName) {
	unsigned int size = sizeof(MEM_BUF)+1024*1024*1024;

	// create shared memory
	if((m_iFd = shm_open(shmName, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1) {
		if(errno == EEXIST) {
			struct stat sb;
			if((m_iFd = shm_open(shmName, O_RDWR, 0)) == -1) {
				perror("[pIPC] <IPCGet> shm_open");
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
			m_pBuf = (MEM_BUF *)mmap64(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_iFd, 0);
			if(m_pBuf == MAP_FAILED) {
				perror("[pIPC] <IPCGet> mmap64");
				return -1;
			}

			printf("[pIPC] Shared memory is opened [name: %s] [size: %d bytes]\n", shmName, size);


			flock(m_iFd, LOCK_UN);
		} else {
			perror("[pIPC] <IPCGet> shm_create");
			return -1;
		}
	} else {
		flock(m_iFd, LOCK_EX);

		if (ftruncate64(m_iFd, size) == -1) {
			perror("[pIPC] <IPCGet> ftruncate64");
			return -1;
		}

		m_pBuf = (MEM_BUF *)mmap64(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_iFd, 0);
		if(m_pBuf == MAP_FAILED) {
			perror("[pIPC] <IPCGet> mmap64");
			return -1;
		}

		printf("[pIPC] <IPCGet> Shared memory is created [name: %s] [size=%d bytes]\n", shmName, size);

		flock(m_iFd, LOCK_UN);
	}
	
	return 0;
}

TREE_NODE* ShmTree::makeNode(const char *_nodeName) {
	TREE_NODE *retNode = new TREE_NODE;
	int subjLen = (int)strlen(_nodeName);                                                
    int tmpTokLen = -1; 

    for (int i=1; i<subjLen; i++) {
		if (*(_nodeName+i) == '/') {
			retNode->cNodeName[retNode->iTokCnt][tmpTokLen+1] = '\0';
			tmpTokLen = -1;
			retNode->iTokCnt++;
			continue;
		}
		tmpTokLen++;
		retNode->cNodeName[retNode->iTokCnt][tmpTokLen] = *(_nodeName+i);
    }       
	retNode->iTokCnt++;

	return retNode;
}

int ShmTree::addNode(TREE_NODE *_node) {
	return 0;
}

void ShmTree::printNode(TREE_NODE *_node) {
	printf("NodeSize[%lu], TokCnt[%d], Token[", sizeof(TREE_NODE), _node->iTokCnt);
	for (int i=0; i<_node->iTokCnt; i++) {
		printf("(%s)", _node->cNodeName[i]);
	}
	printf("]\n");
}

int main() {
	ShmTree tree;
	TREE_NODE *myNode;
	myNode = tree.makeNode("/a/b/c/d");
	tree.printNode(myNode);
	return 0;
}
