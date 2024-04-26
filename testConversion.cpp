#include <iostream>

class Test{
public:
    Test() = default;
    Test(const Test& t) {
        std::cout << "Copy constructor" << std::endl;
    }
    Test(Test&& t) {
        std::cout << "Move constructor" << std::endl;
    }

    int a;
};

Test func(){
    return Test();
}

int main(int argc, const char** argv) {
    int n = 0;
    auto m = n;

    // 万能引用的推导： 
    // T类型的左值被推导为T&类型，T类型的右值被推导为T。 (T 也就是 auto 的部分)
    // 最终结果(加上 auto 后的 &&)： lvalue -> lvalue reference, rvalue -> rvalue reference
    auto&& q = n; 
    auto&& ttt = func();

    // decltype rvalue result is rvalue's type
    decltype(func()) t = func();  // Test

    decltype((func())) t2 = func();  // Test

    // decltype xvalue result is rvalue reference
    decltype(static_cast<Test&&>(t)) t3 = static_cast<Test&&>(t);  // Test&&

    Test tt;
    decltype((tt)) t4 = t; // Test&
    decltype(t) t5 = t;    // Test

    decltype(t.a) k = 0;   // int
    decltype((t.a)) kk = k; // int&

    // decltype lvalue result is lvalue reference ?  not really
    int a = 0,b = 0;
    // a+= 1 is lvalue
    decltype(a+=1) o = b;  // int&
    // a also is lvalue
    decltype(a) x = b;  // int
    decltype((a)) y = b; // int&


    return 0;
}