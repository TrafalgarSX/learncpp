#include <iostream>
#include <string>
#include <algorithm>


// https://blog.csdn.net/u011764940/article/details/106965534

/**
1，重载与using声明(P708)

一个using声明引入的函数将重载该声明语句所属作用域中已有的其他同名函数。

(1)如果using声明出现在局部作用域，则引入的名字将隐藏外层作用域的相关声明。

(2)如果using声明所在的作用域中已经有一个函数与新引入的函数同名且参数列表相同，则该using声明将引发错误(f. using指示)。

(3)除了(1)(2)，using声明将为引入的名字添加额外的重载实例，并最终扩充候选函数集的规模。

2，命名空间隐藏规则的例外---实参相关的查找与类类型形参(P706)   这个知识点很重要

对于命名空间中名字隐藏规则有一个重要例外：当我们给函数传递一个类类型的对象时，除了在常规作用域查找外，**还会查找实参类所属的命名空间**。这一例外对于传递类的引用或指针的调用同样有效。

testClassScope.cpp 测试了这个知识点

3，非模板和模板重载(P616)

当存在多个同样好的函数模板时，编译器选择最特例化的版本，处于同样的原因，一个非模板函数比一个函数模板更好。
*/

#define USE_SELF_SWAP 1
#define USE_HIDE 0

class Hasptr {
public:
    Hasptr(std::string str, int num = 0) : ps(new std::string(str)), i(num) {
        std::cout << "Foo" << std::endl;
    }
#if USE_SELF_SWAP
    // friend void swap(Hasptr&, Hasptr&);
#endif
// private:
    std::string *ps;
    int i;
};
// A
#if USE_SELF_SWAP

#if USE_HIDE
namespace hide {
inline void swap(Hasptr &lhs, Hasptr &rhs)
    {
        using std::swap;
        swap(lhs.ps, rhs.ps); // HasPtr中ps是内置指针, 因此这里调用的是std::swap()
        swap(lhs.i, rhs.i); // HasPtr中i是内置int, 因此这里调用的是std::swap()
    }
}
#else

inline void swap(Hasptr &lhs, Hasptr &rhs)
{
    using std::swap;
    swap(lhs.ps, rhs.ps); // HasPtr中ps是内置指针, 因此这里调用的是std::swap()
    swap(lhs.i, rhs.i); // HasPtr中i是内置int, 因此这里调用的是std::swap()

    std::cout << "Hasptr swap" << std::endl;
}

#endif  // USE_HIDE
#endif // USE_SELF_SWAP
 
class Foo {
public:
    Foo(std::string str, int num = 0) : h(str, num) {
        std::cout << "Foo" << std::endl;
    }

    ~Foo() = default;

    Foo(const Foo& rhs) : h(rhs.h){
    }

    Foo& operator=(Foo rhs){
        std::cout << "Foo operator=" << std::endl;
        swap(*this, rhs);
        return *this;
    }


    friend void swap(Foo&, Foo&);
    Hasptr h;
};
 
#if 0
// B
void swap(Foo &lhs, Foo &rhs)
{
    swap(lhs.h, rhs.h); // 效率低下
}
#else
// C
void swap(Foo &lhs, Foo &rhs)
{
    using std::swap;
    swap(lhs.h, rhs.h); // 会调用HasPtr的swap()
}
#endif



int main() {
    std::cout << "test" << std::endl;

    Foo f1("first", 10);
    Foo f2("second", 20);

    std::cout << *f1.h.ps << " " << f1.h.i << std::endl;
    std::cout << *f2.h.ps << " " << f2.h.i << std::endl;

    f1 = f2;

    std::cout << *f1.h.ps << " " << f1.h.i << std::endl;
    std::cout << *f2.h.ps << " " << f2.h.i << std::endl;
    return 0;
}
