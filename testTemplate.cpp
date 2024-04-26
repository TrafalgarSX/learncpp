#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


template <typename T>
int compare(const T &v1, const T &v2)
{
#if 0
    if (v1 < v2) return -1;
    if (v2 < v1) return 1;
#else
    if (std::less<T>()(v1, v2)) return -1;
    if (std::less<T>()(v2, v1)) return -1;
#endif
    return 0;
}

void test_func_template(){
    int a = 1, b = 2;
    double c = 1.1, d = 2.2;
    std::string e = "hello", f = "world";

    std::cout << compare(a, b) << std::endl;
    std::cout << compare(c, d) << std::endl;
    std::cout << compare(e, f) << std::endl;
}

void test_func_arg(const int& a, int& b, int(&arr)[2], int(*matrix)[2], int *p_arr[2]){
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    for(auto i : arr){
        std::cout << i << " ";
    }

    decltype(matrix) p = &arr;
    decltype(&arr) p2 = &arr;


    int arr2[2]{1,2};
    decltype(arr2) p3 = {4,5};

    decltype(p_arr) p4;
}

// typename 的作用
using size_type = typename std::vector<int>::size_type;
int main()
{
    int arr[2]{1,2};
    int arr2d[2][2]{{1,2},{3,4}};

    test_func_arg(1 + 2, arr[0], arr, &arr, nullptr); // ok
    test_func_arg(1 + 2, arr[0], arr, arr2d, nullptr); // ok

    return 0;
}