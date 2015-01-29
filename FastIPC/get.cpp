#include "ipc.h"

int main() {
	char str[1024];
	long retSize;
	osIPC::Get get("testIPC");

	//for (int i=0; i<100; i++) {
	while (true) {
		retSize = get.read(str, sizeof(str));
		printf("<<[%.*s]\n", (int)retSize, str);
	}
}
