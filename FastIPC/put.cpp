#include "ipc.h"

int main() {
	char str[1024];
	int cnt = 1;
	osIPC::Put put("testIPC");

	//snprintf(str, sizeof(str), "Single Message");
	//client.write(str, strlen(str));

	//for (int i=0; i<100; i++) {
	while (true) {
		snprintf(str, sizeof(str), "%d - Message #%d", getpid(), cnt++);
		put.write(str, strlen(str));
		printf(">>[%.*s]\n", (int)strlen(str), str);
		//sleep(1);
	}
}
