#include <cstdio>
#include <cstring>

void printNum(int i, int numArray[1000]) {
	printf("[2^%d] = ", i);
	for (int i=999; i>=0; i--) {
		printf("%d", numArray[i]);
	}
	printf("\n\n");
}

int main() {
	int numArray[1000] = {1, 0};
	int carry;
	int tmpCarry;
	int digitSum = 0;

	for (int i=0; i<1000; i++) {
		carry = 0;
		for (int j=0; j<1000; j++) {
			if (j != 0 && carry == 1) {
				tmpCarry = 1;
			} else {
				tmpCarry = 0;
			}
			numArray[j] = numArray[j] * 2;
			if(numArray[j] >= 10) {
				carry = 1;
				numArray[j] = numArray[j] - 10;
			} else {
				carry = 0;
			}
			numArray[j] += tmpCarry;
		}
		
		//printNum(i+1, numArray);
	}

	for (int i=0; i<1000; i++){
		digitSum += numArray[i];
	}

	printf("digit's sum: %d\n", digitSum);


	return 0;
}
