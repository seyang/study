#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_num_length(int num) {
    int length = 1;
    while (num /= 10) 
        length++;
    return length;
}

int num_plus(int *num, int plus) {
    int result = -1; 
    for (int i=1; i<=plus; i++) {
        *num += 1;
        if (*num == 1 || *num == 10 || *num == 100 || *num == 1000
                || *num == 10000 || *num == 100000 || *num == 1000000) {
            result = i;
        }   
    }   
    return result;
}

int main() {
    int num = 0;
    int digit = 0;
    int result = 1;
    char tmp_str[16];

    for (int i=1;i<1000000;i++) {
        digit = num_plus(&num, get_num_length(i));
        if (digit > 0) {
            sprintf(tmp_str, "%d", i); 
            result *= (tmp_str[digit-1]-48);
            printf("[ %-8d ] %d'th dight %d\n", i, digit, tmp_str[digit-1]-48);
        }   
    }   
    printf("result %d\n", result);

    return 0;
}
