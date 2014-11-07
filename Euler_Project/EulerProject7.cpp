#include <stdio.h>

#define MAX_NUM 1000000

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
            //printf("%d ", primeSet[i]);
            primeCnt++;
			if (primeCnt == 10001) {
				printf("%d\n", primeSet[i]);
			}
        }   
    }   
    //printf("\n");
    //printf("Count: %d\n", primeCnt);
}

int main() {
	getPrime();

	return 0;
}

