#include <iostream>

class Elem 
{
public:
    Elem(){
        n = 0;
    }
    // explicit Elem(int n) : n(n){
    Elem(int n) : n(n){

    }

    explicit Elem(int n, int m) : n(n){

    }

    explicit Elem(char c) : n(c){

    }
public:
    int n;
};

int main(void){
    // 由于编译器要用初始化器的类型来推导数组元素的类型，只有当初始化器是单一初始化器时，才能推导出数组元素的类型。
    auto p1 = new auto(Elem(1));
    auto p2 = new Elem(Elem(1));


    Elem* arr1 = new Elem[10];  // OK
    // Elem* arr2 = new Elem[];  // wrong
    Elem* arr2 = new Elem[]{1};  // OK
    // Elem* arr = new Elem[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // Elem* arr = new Elem[10](Elem(1)); // error 标准中没有这种写法

    // Elem* arr = new Elem[10]{Elem(1), Elem(2,3), Elem('c')};  // OK
    auto* arr = new Elem[10]{Elem(1), Elem(2,3), Elem('c')};  // OK

    for (int i = 0; i < 10; i++)
    {
        std::cout << arr[i].n << std::endl;
    }
}