#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <set>

int main() {
    std::set<long> myset;
    long num1, num2;
                  
    for (long i=1; i<=10000; i++) {
        myset.insert( i*(i*3-1)/2 );
    }
    
    for (auto it1 = myset.begin(); it1 != myset.end(); it1++) {
        for (auto it2 = myset.begin(); it2 != it1; it2++) {
            num1 = *it1 - *it2;
            num2 = *it1 + *it2;

            if ((myset.find(num1) != myset.end()) && (myset.find(num2) != myset.end())) {
                printf("%ld,%ld -> %ld,%ld\n", *it2, *it1, num1, num2);
                return 0;
            }   
        }   
    }   

    return 0;
}
