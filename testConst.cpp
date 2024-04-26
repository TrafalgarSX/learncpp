#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct Test
{
    int x{0};

    void setx(int num){
        x = num;
    }

};

void test_const_char(){
    const char*  str = "Hello, World!";

    // char* str2 = static_cast<char*>(str); // error

    char* const str3 = "Hello, World!";
    char* str4 = nullptr;

    str4 = const_cast<char*>(str);

    str4[0] = 'g';

    std::cout << str4 << std::endl;
}

void test_const_string(){
    const Test* test = new Test();

    Test* const test1 = new Test();

    Test* test2 = (Test*)(test);
    test2->setx(10);
    std::cout << test2->x << std::endl;
    test2 = const_cast<Test*>(test);
    test2->setx(14);
    std::cout << test2->x << std::endl;
}

void test_rr_reference(){
    int &&rr1 = 42;
    // int &&rr2 = rr1; // error
    int &&rr2 = (int&&)rr1;
    int &&rr3 = static_cast<int&&>(rr1);
    int &&rr4 = std::move(rr1);
}

class Foo{
public:
    Foo sorted() &{ std::cout << "lvalue reference" << std::endl; return *this;}
    Foo sorted() &&{std::cout << "rr reference" << std::endl; return *this;}
    Foo sorted() const &{ std::cout << "const lvalue reference" << std::endl; return *this;}

    static Foo& test_static(){
        static Foo f;
        return f;
    }

    void test(Foo& f){
        f.data.push_back(1);
    }
private:
    std::vector<int> data;
};

void test_auto_const(){
    int i = 3;
    int &j = i;
    const int ci = i;
    auto a = &ci;
    auto b = &i;

    auto &c = ci;
    auto d = ci;
    const auto& e = 42;

    auto f = j;


    decltype(ci) x = 0;
    decltype(j) y = i;
    decltype((j)) z = i;
    decltype((i)) q = i;

    int *p = &i;
    decltype(*p) u = i;



    Foo test;
    decltype(Foo::test_static()) r = test;

}

// https://andreasfertig.blog/2024/04/understanding-the-role-of-cv-qualifiers-in-function-parameters/
// You cannot have function overloads based on top-level cv-qualifiers because C++ omits top-level cv-qualifiers in a function's signature.
void Fun(int){

}
#if 0
void Fun(const int){

}
#endif

void Func(int*){
    std::cout << "int*" << std::endl;
}
#if 0
void Func(int* const){
    std::cout << "int* const" << std::endl;
}
#endif

int main()
{
    // const_cast不能用于改变指针本身的const限定(top const)，只能用于改变指针所指向的对象的const限定(low const)。
    int num;
    const int num2 = 2;

    int* const p = &num;

    // ++(const_cast<int*>(p));  // error lvalue casts are not supported
    // ++((int*)(p));  // error lvalue casts are not supported

    auto q1 = &num;
    auto q2 = &num2;

    const int* q = &num2;
    int* q3 = const_cast<int*>(q);

    return 0;
}

