#include <iostream>


// 
namespace A{
    char ch = 'A';
    std::string a_str = "GA";
    void test_func_using(){ std::cout << " A f()" << std::endl;};
}

char ch = 'G';

namespace B{

    namespace BA{
        char ch = 'A';
        std::string a_str = "BA";
        void test();
    }

    char ch = 'B';
    void f(){
        #if 1

        std::string a_str = "func a_str";
        using namespace BA; // 功能有两个： 1. 将命名空间成员提升到包含命名空间本身(不同 using 声明) 2. using 指示的最近作用域的效果(同 using 声明)
        #else
        using A::ch;  // using 声明功能更加单一: 简单的令名字在局部作用域内可见， 会隐藏外部同名变量
        using A::a_str;
        #endif
        /**
        "using指示具有将命名空间成员提升到包含命名空间本身"

        这句话的详细解释应该是： 将命名空间成员提升到 **包含命名空间定义本身的命名空间**

        比如这里的例子： using namespace A; 是将 namespace A 中的所有成员提升到 B 的作用域
        也就是包含了 namespace A 定义的 命名空间 B
        
        */
        // std::cout << ch << std::endl;
        std::cout << a_str << std::endl;
    }
}

void f(){
    using namespace A;
    /**
        using指示具有将命名空间成员 **提升到包含命名空间本身** 和 **using指示的最近作用域的效果**
        "using指示的最近作用域的效果"  这个很好理解

        如何理解 "将命名空间成员提升到包含命名空间本身" 呢？
    */
    // std::cout << ch << std::endl; // error

    std::cout << "G f()" << std::endl;
}

class Father{
public:

    // Using declaration in class refers into std::, which is not a class
    // cpp primer 中说，using **声明**, 不能用于类外部
    // using std::swap; // error  
    // cpp primer 中说，using **指示**, 不能用于类外部
    // using namespace std; // error 

    // using 声明 只引入一个， using 指示 引入所有

    void print(int a, int b){
        // 这个 using 声明是在函数内部的，所以只在函数内部有效, 不是在类内部
        using std::swap;
        std::cout << "Father" << std::endl;
        swap(a, b);
        std::cout << "a is :" << a << std::endl;
        std::cout << "b is :" << b << std::endl;
    }

};

class Son : public Father{

};

void test_func_using(){ std::cout << " G f()" << std::endl;};

namespace A
{
    void test(int) {}
}

#if 0
using A::test;
void test(int) {}    // 编译错误
#else
using namespace A;
void test(int) {}    // 可以通过编译， 但是调用的时候还是  ambiguous, 需要明确调用哪一个
#endif

int main(int argc, const char** argv) {
    Father father;

    father.print(10, 20);
#if 0 
    test(1);
    test_func_using();

    std::cout << ch << std::endl;  // error
#endif

    B::f();
    return 0;
}


