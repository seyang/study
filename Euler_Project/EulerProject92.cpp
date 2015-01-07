#include <stdio.h>
#include <unistd.h>

int main() {
	int chk_num;
	int digit;
	int digit_square_sum;
	int num_cnt = 0;;
	for (int i=1; i<10000000; i++) {
		chk_num = i;
		digit_square_sum = 0;
		
		while (true) {
			digit = chk_num % 10;
			digit_square_sum += (digit * digit);
			if (digit_square_sum == 1 && chk_num < 10) {
				break;
			}
			if (digit_square_sum == 89 && chk_num < 10) {
				num_cnt++;
				break;
			}
			chk_num = chk_num / 10;
			if (chk_num == 0) {
				chk_num = digit_square_sum;
				digit_square_sum = 0;
			}
		}
	}
	printf("cnt: %d\n", num_cnt);

	return 0;
}
