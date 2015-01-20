#include <stdio.h>
#include <unistd.h>

#define MAX_NUM 2000000

int getPrime(int primeSet[]) {
	int _tmpSet[MAX_NUM];
	int i;
	int primeCnt = 0;

	for (i=2; i<MAX_NUM; i++) {
		_tmpSet[i] = i;
	}

	for (i=2; i<MAX_NUM; i++) {
		if (_tmpSet[i] == 0) {
			continue;
		}
		for (int j=i+i; j<MAX_NUM; j+=i) {
			_tmpSet[j] = 0;
		}
		primeSet[primeCnt++] = i;
	}

	return primeCnt;
}

int main() {
	int primeCnt;
	int primeSet[1000000];
	long primeSum = 0;

	primeCnt = getPrime(primeSet);

	for (int i=0; i<primeCnt; i++) {
		primeSum += primeSet[i];
	}

	printf("primeSum: %ld\n", primeSum);

	return 0;
}

