//#include "precomp.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <tr1/unordered_map>

using namespace std;

unsigned long hash(unsigned char *str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++) != '\0') {
		hash = ((hash << 5) + hash) + c;
		//cout << hash << endl;
	}

	return hash;
}
int main() {
	char str[128] = "test";
	unsigned long num1 = hash((unsigned char *)str);

	cout << num1 << endl;
	
	return 0;
}
