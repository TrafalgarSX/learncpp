#include <iostream>

#if 0
template<typename T> 
bool compare(const T& a, const T& b)
{
    int size = sizeof(a);
    return a < b;
}
#endif

template<typename T> 
bool compare(T a, T b)
{
    int size = sizeof(a);
    return a < b;
}


template<class T> struct remove_reference 
{
    typedef T type;
};

#if 0

template<class T> struct remove_reference<T&>
{
    typedef T type;
};

template<class T> struct remove_reference<T&&>
{
    typedef T type;
};

#endif

template<class T> 
void remove_reference_func(T t)
{
    typedef T type;

    type x = t;

    x = 10;
}


int main(int argc, const char** argv) {

    int i;

    decltype(i) ii = 0;


    typename remove_reference<decltype((i))>::type jj = i;


    remove_reference_func(i);

    remove_reference_func<int&>(i);


    remove_reference<decltype(i)>::type j = 0;


    remove_reference<decltype(42)>::type x = 0;

    decltype(std::move(i)) z = 0;

    remove_reference<decltype(std::move(i))>::type y = 0;


    remove_reference_func(std::move(i));



    int ia[] = {1,2,3,4,5,6};
    int* bp = std::begin(ia);
    int* ep = std::end(ia);

    int ib[] = {1,2,3,4,5,6};

    std::cout << std::equal(bp, ep, ib) << std::endl;

    // bool ret = compare(ia, ib);

    compare(i, ii);

    decltype(ia) ic = {1,2,3,4,5,6};
    decltype((ia)) id = ic;
    decltype(&ia) ie = &ic;
    

    return 0;
}
