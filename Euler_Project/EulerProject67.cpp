#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 100
#define max(a,b) (((a) > (b)) ? (a) : (b))

void print_array(int num[MAX_LINE][MAX_LINE]) {
	for (int i=0; i<MAX_LINE; i++) {
		for (int j=0; j<=i; j++) {
			printf("%04d ", num[i][j]);
		}
		printf("\n");
	}
}

int main() {
	FILE *fp;
	int num[MAX_LINE][MAX_LINE] = {{0}};

	if ((fp = fopen("EulerProject67.data", "r")) == NULL) {
		perror("fopen");
		exit(1);
	}

	for (int i=0; i<MAX_LINE; i++) {
		for (int j=0; j<=i; j++) {
			fscanf(fp, "%d", &num[i][j]);
		}
	}

	for (int i=1; i<MAX_LINE; i++) {
		for (int j=0; j<=i; j++) {
			if (j == 0) {
				num[i][j] = num[i][j] + num[i-1][j];
				continue;
			}
			num[i][j] = num[i][j] + max(num[i-1][j-1], num[i-1][j]);
		}
	}
	//print_array(num);

	int max = 0;
	for (int i=0; i<MAX_LINE; i++) {
		if (num[MAX_LINE-1][i] > max) {
			max = num[MAX_LINE-1][i];
		}
	}

	printf("Answer: %d\n", max);

	fclose(fp);
	return 0;
}
