#include <iostream>
#include <cstdarg>

void testArg(int n, ...)
{
    va_list ap;
    va_start(ap, n);
    for (int i = 0; i < n; i++)
    {
        int arg = va_arg(ap, int);
        std::cout << arg << std::endl;
    }
    va_end(ap);
}

int main(void)
{
    testArg(5, 1, 2, 3, 4, 5);
    return 0;
}