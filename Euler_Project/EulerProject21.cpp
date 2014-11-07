#include <stdio.h>

int d(int num) {
	int ret = 0;
	for (int i=1; i<num; i++) {
		if (num%i == 0) {
			ret += i;
		}
	}
	return ret;
}
int main() {
	int d_num1;
	int d_num2;
	int sum = 0;
	for (int i=2; i<10000; i++) {
		d_num1 = d(i);
		d_num2 = d(d_num1);
		if (d_num2 == i && d_num1 != i && d_num1 > i) {
			printf("%d, %d\n", d_num1, i);
			sum += i;
		}
	}

	printf("sum: %d\n", sum);

	return 0;
}
