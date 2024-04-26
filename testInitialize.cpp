#include <iostream>

class Test{
public:
    // 用户自定义的默认构造函数 会使得默认初始化时，成员变量的值不确定
    // 或者没有默认构造函数， 或者被 delete 的默认构造函数， 都会使得默认初始化时，成员变量的值不确定
    Test(){}

    int a;
};

class Test2{
public:
    // Test() = default;
    int a;

};


int main(int argc, const char** argv) {
    // 默认初始化无论是否有自定义的 默认构造函数， 其成员的值都是不确定的
    Test t;     // 默认初始化
    Test2 t2;   // 默认初始化
    Test* pt = new Test;  // 默认初始化

    // 这种后面有 {}  () 的初始化方式叫值初始化， 里面可以有参数， 
    // 值初始化对于没有自定义默认构造函数的类， 会零初始化，否则会默认初始化（值不确定）
    Test t3{};
    Test2 t4{};
    Test* pt2 = new Test();  
    Test2* pt3 = new Test2();  
    return 0;
}