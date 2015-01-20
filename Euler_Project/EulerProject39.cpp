#include <stdio.h>
#include <math.h>

int main() {
	int pCnt[1001] = {0};
	int maxP = 0;
	for (int i=1; i<500; i++) {
		for (int j=1; j<500; j++) {
			for (int k=1; k<500; k++) {
				if ((i+j+k) <= 1000) {
					if (pow(i,2) == (pow(j,2)+pow(k,2))) {
						pCnt[i+j+k]++;
					}
				}
			}
		}
	}
	for (int i=0; i<1001; i++) {
		if(pCnt[i] > maxP) {
			maxP = i;
		}
	}
	printf("MAX P: %d\n", maxP);
	return 0;
}
