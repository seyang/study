#include <stdio.h>

int main() {
	int max = -1;
	int temp;
	int cnt;
	for (int i=1; i<1000000; i++) {
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
		}
	}
	printf("max collatz num: %d\n", max);
	return 0;
}
