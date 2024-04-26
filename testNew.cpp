#include <iostream>

/**
1. std::ostream& operator<<(std::ostream& os, const T& obj) 为什么这个函数不能以成员函数实现？
    因为它们接收用户定义类型为右实参（a@b 中的 b），所以它们必须以非成员实现。 
    左操作数：在C++中，成员函数的调用总是通过它的左操作数（也就是调用它的对象）来进行的。在os << obj这个表达式中，左操作数是os，它是一个std::ostream对象，而不是你的自定义类型T的对象。因此，你不能将operator<<作为T的成员函数来实现。

2. void operator delete ( void* ptr, void* place ) noexcept; 这个函数的作用叫什么？
    这个函数是C++的一个定位删除运算符（placement delete operator）。它的主要用途是配合定位new运算符（placement new operator）使用。
    定位new运算符允许你在已经分配的内存上构造一个对象。然而，如果对象的构造函数抛出一个异常，那么C++运行时需要一种方法来正确地释放那块内存。

    定位删除运算符的主要目的是处理构造函数抛出的异常。如果定位new运算符在构造对象时抛出异常，那么C++运行时会调用定位删除运算符来销毁部分构造的对象。相比之下，当你直接调用类的析构函数时，你需要自己处理可能的异常。

    cppreference: 
    operator delete 与 operator delete[] 带额外用户定义形参的重载（“布置形式”，(15,16)）可照常在全局作用域声明，而且会被匹配的布置形式 new 表达式 
    **在所分配的对象的构造函数抛出异常时调用。 **

3. 当  void operator delete(void* ptr)  和 void operator delete(void* ptr, std::size_t sz) 两个函数都存在时，delete p; 会调用哪个？
    当两个函数都存在时，delete p; 会调用 void operator delete(void* ptr)。因为delete p; 会调用void operator delete(void* ptr, std::size_t sz)这个函数，但是这个函数的第二个参数是隐式的，所以会调用void operator delete(void* ptr)。

4. void operator delete(void* ptr, std::size_t sz) 和 void operator delete[](void* ptr, std::size_t sz) 两个函数的作用是什么？
   其实就是在删除的时候可以获得实际删除对象的大小，这样可以根据对象的大小来进行一些操作，比如统计已经删除的对象的大小等等。
   尤其是删除继承体系中的对象时，可以获得对象的大小。

5. 定义在类中的 operator delete/new 和 operator delete/new[]  会被调用吗？
    不会。如果类定义了 operator delete 或 operator delete[]，那么它们只能用于删除类的对象，而不能用于删除类的派生类的对象。如果你想要删除类的派生类的对象，那么你必须在派生类中定义 operator delete 或 operator delete[]。
    如果类定义了 operator delete 或 operator delete[]，那么它们只能用于删除类的对象，而不能用于删除类的派生类的对象。如果你想要删除类的派生类的对象，那么你必须在派生类中定义 operator delete 或 operator delete[]。

    而且定义在类中的 operator delete/new 和 operator delete/new[] 是隐式 static 的, 原因在 cpp primer 727 页有解释

6. 值得多提一嘴的是，虽然operator delete重载和operator new相同，但一般不会重载operator delete。

    根本原因是重载后的delete不可手动调用。例如：

    void operator delete(void* ptr， int num)
    {
    std::cout << "operator delete" << std::endl;
    std::free(ptr);
    }

    delete(10) p;    // 不合法的

    这样调用是不合法的。

    这种重载的意义是和重载operator new配套。只有operator new报异常了，就会调用对应的operator delete。若没有对应的operator delete，则无法释放内存。

7. cpp primer p727 说 operator new 不能被重载成 void* operator new(std::size_t size, void* ptr) noexcept, 为什么？
   实际上是 cpp primer 表达的不准确，operator new 是不可以重载成 void* operator new(std::size_t size, void* ptr) noexcept, 因为这个符号已经被 
   C++ 标准库使用， 否则就重定义了， 其他的都可以重载， 而且类中的 operator new 可以重载成 placement new 的形式， 因为这时候不会找到标准库的实现。

*/

// 指定大小的类特定解分配函数
#include <iostream>

struct X
{
    static std::size_t total_deleted;

    static void operator delete(void* ptr, std::size_t sz)
    {
        total_deleted += sz;
        std::cout << "Deleting object of size: " << sz << '\n';
        std::cout << "Total deleted so far: " << total_deleted << '\n';
        ::operator delete(ptr);
    }

    static void operator delete[](void* ptr, std::size_t sz)
    {
        total_deleted += sz;
        std::cout << "Deleting array of size: " << sz << '\n';
        std::cout << "Total deleted so far: " << total_deleted << '\n';
        ::operator delete(ptr);
    }
    int a{10};
};

std::size_t X::total_deleted = 0;

class Base
{
public:
    virtual ~Base() = default;
#if 0
    static void operator delete(void* ptr){
        std::cout << "Base::operator delete" << std::endl;
        ::operator delete(ptr);
    }
#endif
    static void operator delete(void* ptr, std::size_t sz)
    {
        std::cout << "Deleting object of size: " << sz << '\n';
        ::operator delete(ptr);
    }
};

class Derived : public Base
{
public:
    int arr[10]; // 增加额外的数据成员，使得Derived的大小大于Base
};

class Foo {
public:
 void* operator new(std::size_t size, void* ptr, const std::string& str)
 {
     std::cout << "placement new: " << str << std::endl;
  return ptr;
 }

 int a{10};
};
//  void* operator new(std::size_t size, void* ptr) noexcept
 void* operator new(std::size_t size, void* ptr, int a) noexcept
 {
     std::cout << "placement new: " << std::endl;
  return ptr;
 }

void operator delete(void* ptr, int num)
{
    std::cout << "operator delete" << std::endl;
    std::free(ptr);
}

#if 1
int main()
{
    Base* p = new Derived;
    delete p; // 调用Base::operator delete

    Base* p2 = new Base;
    delete p2;

    uint8_t buffer[sizeof(Foo) + 4];
    std::string test = "test";
    Foo* p3 = new (buffer, test) Foo;  // 
    std::cout << "p3->a: " << p3->a << std::endl; // 

    int* p4 = new (buffer + sizeof(Foo), 10) int;

    // delete(10) p4; // error 不能手动调用 operator delete 的重载


}
#else
int main()
{
    X* p1 = new X;
    delete p1;

    X* p2 = new X[10];
    // delete[] p2;
}
#endif


/**

    No, you cannot call delete in that way. The delete operator does not take a string argument. It only takes a pointer to the memory that was allocated with new.

    If you have overloaded the delete operator in your class to take additional arguments, those arguments cannot be passed directly to delete. The additional arguments are only used when the delete operator is called implicitly to destroy an object of your class.

    Here's an example:
    ```cpp
    class MyClass {
public:
    // Overloaded new operator
    static void* operator new(std::size_t size, const std::string& str) {
        std::cout << "Overloaded new operator with string: " << str << "\n";
        return ::operator new(size);
    }

    // Overloaded delete operator
    static void operator delete(void* ptr, const std::string& str) {
        std::cout << "Overloaded delete operator with string: " << str << "\n";
        ::operator delete(ptr);
    }
};

int main() {
    MyClass* obj = new("hello") MyClass; // This is fine
    delete("hello") obj; // This is NOT allowed
}
```
In this example, new("hello") MyClass is fine because it calls the overloaded new operator with the string "hello". However, delete("hello") obj is not allowed because delete does not take additional arguments like new does. The overloaded delete operator with the string argument would only be called if the new operator throws an exception during object construction.

*/