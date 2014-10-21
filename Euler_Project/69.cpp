#include <stdio.h>

#define MAX_NUM 100

int gcd(int p, int q) {
	while (q > 0) {
		int c = q;
		q = p % q;
		p = c;
	}
	return p;
}

int primeSet[MAX_NUM];

void getPrime() {
	int i, j;
	int primeCnt = 0;

	for (i=2; i<MAX_NUM; i++) {
		primeSet[i] = i;
	}

	for (i=2; i<MAX_NUM; i++) {
		if (primeSet[i] == 0) {
			continue;
		}
		for (int j=i+i; j<MAX_NUM; j+=i) {
			primeSet[j] = 0;
		}
	}

	for (i=0; i<MAX_NUM; i++) {
		if (primeSet[i] != 0) {
			printf("%d ", primeSet[i]);
			primeCnt++;
		}
	}
	printf("\n");
	printf("Count: %d\n", primeCnt);
}

int main() {
	int maxN;
	double maxValue=0;
	int tmpPhi;
	double tmpMax;
	int tmpGcd;
	
	getPrime();

	//for (int i=1; i<1000000; i++) {
		//tmpPhi = 1;
		//for (int j=2; j<i; j++){
			//if (gcd(i, j) == 1) {
				//tmpPhi++;
			//}
		//}
		////tmpMax = (double)i/(double)tmpPhi;
		////if (maxValue < tmpMax) {
			////maxValue = tmpMax;
			////maxN = i;
		////};
		////printf("n: %d, phi(n): %d, maxValue: %f\n", i, tmpPhi, maxValue);
	//}

	//printf("Max value: %f\n", maxValue);

	return 0;
}
