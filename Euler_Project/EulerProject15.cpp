#include <stdio.h>

long chk_func(long size) {
	long arr[size+1][size+1];

	for (long i=0; i<size+1; i++) {
		arr[i][0] = 1;
		arr[0][i] = 1;
	}

	for (long i=1; i<size+1; i++) {
		for(long j=1; j<size+1; j++) {
			arr[i][j] = arr[i-1][j] + arr[i][j-1];
		}
	}

	printf("Answer: %ld\n", arr[size][size]);

	return 0;
}

int main() {
	chk_func(20);

	return 0;
}
