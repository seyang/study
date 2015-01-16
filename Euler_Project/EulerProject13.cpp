#include <stdio.h>
#include <string.h>

#define MAX_DIGIT 60
#define NUM_CNT 100 

int makeCharNum(char *num) {
	int nullCharPos = 0;
	int movePos = 0;

	for (int i=0; i<(MAX_DIGIT+1); i++) {
		if (num[i] == '\0') {
			nullCharPos = i;
			break;
		}
	}
	movePos = MAX_DIGIT-nullCharPos;
	for (int i=(nullCharPos-1); i>-1; i--) {
		num[i+movePos] = num[i];
		num[i] = '0';
	}
	num[MAX_DIGIT] = '\0';
	return 0;
}

int charNumSum(char *num1, char *num2, char *sum) {
	int carry;

	memset(sum, '0', MAX_DIGIT+1);
	sum[MAX_DIGIT] = '\0';

	for (int i = MAX_DIGIT-1; i>-1; i--) {
		carry = (num1[i] - '0') + (num2[i] - '0') + (sum[i] - '0');
		if (carry >= 10) {
			sum[i-1] = sum[i-1] + 1;
			sum[i] = '0' + carry - 10;
		} else {
			sum[i] = '0' + carry;
		}
	}
	return 0;
}

int main() {
	char num[NUM_CNT][MAX_DIGIT+1];
	char sum[MAX_DIGIT+1];
	char tmpNum[MAX_DIGIT+1];
	int num_cnt = 0;
	FILE *fp;

	memset(num, '0', NUM_CNT * (MAX_DIGIT+1));
	memset(sum, '0', MAX_DIGIT+1);
	memset(tmpNum, '0', MAX_DIGIT+1);

	sum[MAX_DIGIT] = '\0';
	tmpNum[MAX_DIGIT] = '\0';
	
	if ((fp = fopen("EulerProject13.data", "r")) == NULL) {
		perror("Open fp");
	}

	while (fscanf(fp, "%s", num[num_cnt]) != EOF) {
		makeCharNum(num[num_cnt]);
		memcpy(tmpNum, sum, MAX_DIGIT+1);
		charNumSum(num[num_cnt], tmpNum, sum);
		num_cnt++;
	}

	printf("Sum: %s\n", sum);
}
