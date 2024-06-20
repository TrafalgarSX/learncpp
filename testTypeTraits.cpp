#include <iostream>
#include <string>
#include <vector>

class TestClass{
public:
    using Type = std::string;
};

template<typename T>
struct TestTraits{
    using Type = typename T::Type;
};

// partial specialization for pointer types
template<typename T>
struct TestTraits<T*>{
    using Type = T;
};

int main(int argc, const char** argv) {
    TestTraits<int*> t2;

    typename TestTraits<TestClass>::Type str = "Hello";

    typename TestTraits<int*>::Type i = 10;

    std::cout << str << std::endl;
    std::cout << i << std::endl;

    return 0;
}