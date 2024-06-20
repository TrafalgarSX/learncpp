#include <iostream>
#include <functional>

class MyClass {
public:
    void print(int x) {
        std::cout << "Value: " << x << std::endl;
    }
};

int main() {
    MyClass obj;

    // 使用std::bind创建一个可调用对象
    auto func = std::bind(&MyClass::print, &obj, std::placeholders::_1);

    // 现在我们可以像调用普通函数一样调用func
    func(10);

    auto f1 = std::bind(&MyClass::print, std::ref(obj), std::placeholders::_1);
    f1(10);

    auto f2 = std::bind(&MyClass::print, std::placeholders::_1, std::placeholders::_2);
    f2(obj, 10);
    f2(&obj, 10);

    // 使用std::function封装成员函数
    std::function<void(int)> func2 = std::bind(&MyClass::print, &obj, std::placeholders::_1);
    // 同样，我们可以像调用普通函数一样调用func2
    func2(20);

    std::function<void(MyClass&, int)> ff = &MyClass::print;
    ff(obj, 20);

    std::function<void(MyClass*, int)> ff2 = &MyClass::print;
    ff2(&obj, 20);

    auto func3 = std::mem_fn(&MyClass::print);
    func3(obj, 30);
    func3(&obj, 40);



    return 0;
}