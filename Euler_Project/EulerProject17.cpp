#include <stdio.h>
#include <string.h>

char num_0to9[][128] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
char num_10to19[][128] = {"ten", "eleven", "twelve", "thirteen", "fourteen"
    , "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
char num_tens[][128] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

char *func_1to99(int _num, char *_str) {
    if (_num < 10) {
        sprintf(_str, "%s", num_0to9[_num]);
    } else if (_num < 20) {
        sprintf(_str, "%s", num_10to19[_num-10]);
    } else if (_num < 100) {
        int tens_num = _num/10;
        if (_num%10 == 0) {
            sprintf(_str, "%s", num_tens[tens_num]);
        } else {
            sprintf(_str, "%s%s", num_tens[tens_num], num_0to9[_num%10]);
        }   
    }   

    return _str;
}
        
char *func_100to999(int _num, char *_str) {
    int hundreds_num = _num/100;
    char hundreds_str[1024];
    char temp_str[1024];

    sprintf(hundreds_str, "%shundred", num_0to9[hundreds_num]);

    if (_num % 100 == 0) {
        sprintf(_str, "%s", hundreds_str);
    } else {
        sprintf(_str, "%sand%s", hundreds_str, func_1to99(_num%100, temp_str));
    }   
        
    return _str;
}

int main() {

    char num_str[1024];
    long letters_sum = 0;
    char temp_str[1024];

    for (int i=1; i<=1000; i++) {
        if (i < 100) {
            sprintf(num_str, "%s", func_1to99(i, temp_str));
        } else if (i < 1000) {
            sprintf(num_str, "%s", func_100to999(i, temp_str));
        } else if (i == 1000) {
            sprintf(num_str, "onethousand");
        } else {
            sprintf(num_str, "Undefined");
        }
        //printf("[(%04d) %-30s] -> %ld\n", i, num_str, strlen(num_str));
        letters_sum += strlen(num_str);
    }

    printf("letters_sum = [%ld]\n", letters_sum);
    return 0;
}
