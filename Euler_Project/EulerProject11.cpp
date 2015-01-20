#include <stdio.h>

int main() {
	int num[20][20];
	int tmp_num;
	int max = 0;

	FILE *fp;
	if ((fp = fopen("EulerProject11.data", "r")) == NULL) {
		perror("fopen");
	}

	for (int i=0; i<20; i++) {
		for (int j=0; j<20; j++) {
			fscanf(fp, "%d", &num[i][j]);
		}
	}

	for (int i=0; i<20; i++) {
		for (int j=0; j<17; j++) {
			tmp_num = num[i][j] * num[i][j+1] * num[i][j+2] * num[i][j+3];
			
			if (tmp_num > max) max = tmp_num;
		}
	}
	for (int i=0; i<17; i++) {
		for (int j=0; j<20; j++) {
			tmp_num = num[i][j] * num[i+1][j] * num[i+2][j] * num[i+3][j];
			
			if (tmp_num > max) max = tmp_num;
		}
	}
	for (int i=0; i<17; i++) {
		for (int j=0; j<17; j++) {
			tmp_num = num[i][j] * num[i+1][j+1] * num[i+2][j+2] * num[i+3][j+3];
			
			if (tmp_num > max) max = tmp_num;
		}
	}
	for (int i=3; i<20; i++) {
		for (int j=0; j<17; j++) {
			tmp_num = num[i][j] * num[i-1][j+1] * num[i-2][j+2] * num[i-3][j+3];
			
			if (tmp_num > max) max = tmp_num;
		}
	}
	printf("Max: %d\n", max);
}
