#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

#define MAX_DIGIT 512

int factSet[10];

int factorial(int _num) {
	if (_num == 1 || _num == 0)
		return 1;
	return _num*factorial(_num-1);
}

void setFactorial() {
	for (int i=0; i<10; i++) {
		factSet[i] = factorial(i);
	}
}

int factorialSum(int _checkNum) {
	int retNum = 0;
	while (_checkNum != 0) {
		retNum += factSet[_checkNum%10];
		_checkNum = _checkNum/10;
	}
	return retNum;
}

int main() {
	int checkNum = 3;
	int fSum;

	setFactorial();

	//while (true) {
		//fSum = factorialSum(checkNum);
		//if (checkNum == fSum) {
			//printf("%d -> %d \n", checkNum, fSum);
		//}
		//checkNum++;
	//}

	int aNum = 9;
	while(aNum < factorialSum(aNum)) {
		printf("%d -> %d\n", aNum, factorialSum(aNum));
		aNum = aNum * 10 + 9;
		sleep(1);
	}
	return 0;
}
