#include <stdio.h>

#define MAX_CNT 4000000

int main() {
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int sum = 2;


	while (true) {
		num3 = num1 + num2;
		if (num3 > MAX_CNT) {
			break;
		}
		num1 = num2;
		num2 = num3;
		if (num3 % 2 == 0) {
			sum += num3;
		}
	}
	printf("Sum: %d\n", sum);
	return 0;
}
