#include <stdio.h>
#include <unistd.h>

int main() {
	long max = -1;
	long temp;
	long cnt;
	long collatz = 0;
	for (long i=1; i<1000000; i++) {
		temp = i;
		cnt = 1;
		while (temp != 1) {
			if (temp & 1) {
				temp = temp * 3 + 1;
			} else {
				temp = temp >> 1;
			}
			cnt ++;
		}

		if (cnt > max) {
			max = cnt;
			collatz = i;
		}
	}
	printf("max collatz chain: %ld\n", max);
	printf("max collatz num: %ld\n", collatz);
	return 0;
}
