#include <stdio.h>
#include <unistd.h>

int MyCallBack(void (*callback)(const char* msg), const char* msg) {
	callback(msg);
	return 0;
}

void printTest1(const char* msg) {
	for (int i=0; i<3; i++) {
		printf("Test1: %s\n", msg);
	}
}

void printTest2(const char* msg) {
	for (int i=0; i<3; i++) {
		printf("Test2: %s\n", msg);
	}
}

void printTest3(const char* msg) {
	for (int i=0; i<3; i++) {
		printf("Test3: %s\n", msg);
	}
}

void (*callback)(const char* msg);

int main() {
	MyCallBack(printTest1, "Msg1");
	printf("--------------\n");
	MyCallBack(printTest2, "Msg2");
	printf("--------------\n");
	callback = printTest3;
	callback("Msg3");
}
