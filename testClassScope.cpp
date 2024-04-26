#include <iostream>
#include <string>

/*
Argument Dependent Lookup  专业术语
https://zh.cppreference.com/w/cpp/language/adl


实参依赖查找（argument-dependent lookup），又称 ADL 或 Koenig 查找 [1]，是一组对
**函数调用表达式（包括对重载运算符的隐式函数调用）**中的 **无限定的函数名** 进行查找的规则。

在通常 **无限定名字** 查找所考虑的作用域和命名空间之外，还会在它的各个实参的命名空间中查找这些函数。

下面这条很有意思。
ADL 能找到完全在类或类模板内定义的友元函数（典型的例子是重载的运算符），即使它始终未在命名空间层次进行声明。 

TODO 理解友元函数的意义和作用

命名空间隐藏规则的例外---实参相关的查找与类类型形参(P706)   这个知识点很重要

对于命名空间中名字隐藏规则有一个重要例外：当我们给函数传递一个类类型的对象时，除了在常规作用域查找外，**还会查找实参类所属的命名空间**。这一例外对于传递类的引用或指针的调用同样有效。
*/

namespace class_scope {

struct Test
{
    int x{0};

    void setx(int num){
        x = num;
    }

/**
    在类内部定义的友元函数可以访问类的私有成员，但这并不意味着它是类的成员函数。
    它仍然是一个非成员函数，只是被类授予了访问其私有和保护成员的权限。
    因此，它不能使用类的this指针，也不能直接访问类的非静态成员。
*/
    friend void test_frient(){
        // this.x = 10;  // error 
        // x = 10;  // error
    }
};

std::string class_inner =  "same scope with class_scope";

std::string test(Test t){
    std::cout << "inner class Test scope" << std::endl;
    return class_inner;
}

}

using class_scope::Test;

void test(Test t){
    std::cout << "outer class Test scope" << std::endl;
}

int main()
{
    Test t;
    class_scope::test(t);
    ::test(t);
    // test(t);  // Call to 'test' is ambiguous    
    int nums[10];
    decltype(nums) arr{0};
    decltype(&nums) pp = nullptr;
    decltype(&(nums))  pp2 = nullptr;

    long long * p = nullptr;
    p = reinterpret_cast<long long*>(&p);

    std::cout << std::uppercase << std::showbase;

    std::cout <<"pointer " << std::hex << *p << "; pointer self" << p << std::endl;
    std::cout <<"pointer address" << std::hex << &p << "; pointer self" << p << std::endl;


    std::cout << "express nums address:" << &(p) << std::endl;

    std::cout <<  "array address" << std::hex << &arr << "; array self" << arr << std::endl;

    return 0;
}