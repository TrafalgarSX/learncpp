#include <iostream>

int main()
{
    volatile int a = 10;
    // int *p = &a;  // error: invalid conversion from 'volatile int*' to 'int*'
    int* q = reinterpret_cast<int*>(a);

    int *volatile vip;
    vip = q;

    return 0;
}