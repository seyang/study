/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name: EulerProject69.cpp
* Purpose: Euler Project No.69
* Creation Date: 21-10-2014
* Created By: Seung-Eun Yang
_._._._._._._._._._._._._._._._._._._._._.*/

#include <stdio.h>
#include <unistd.h>

#define MAX_NUM 1000000

int getPrime(int primeSet[]) {
	int _tmpSet[MAX_NUM];
	int i, j;
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
	volatile int resultNum=1;
	int primeCnt;
	int primeSet[1000000];
	int _tmpCnt;

	primeCnt = getPrime(primeSet);

	while ((resultNum * primeSet[_tmpCnt]) < MAX_NUM) {
		resultNum *= primeSet[_tmpCnt];
		_tmpCnt++;
		//printf("result: %d, count: %d\n", resultNum, _tmpCnt);
	}
	printf("result: %d, count: %d\n", resultNum, _tmpCnt);

	return 0;
}

