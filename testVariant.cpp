#include <iostream>
#include <string>
#include <variant>


struct {
    void operator()(int num) { std::cout << "int!:"<< num << "\n"; }
    void operator()(const std::string &str) { std::cout << "string!" << str <<" \n"; }
} visitor;

int main()
{
    std::variant<std::string, int> data;

    data = "Cherno";
    std::cout << std::get<std::string>(data) << std::endl;

    data = 2;
    std::cout << std::get<int>(data) << std::endl;

    std::variant<std::string, int> anotherData;

    // get_if 返回指向数据的指针，如果数据类型不匹配，则返回 nullptr
    anotherData = "Cherno";
    int *value = std::get_if<int>(&anotherData);
    if(value != nullptr){
        std::cout << *value << std::endl;
    }else
    {
        std::cout << "Not Found" << std::endl;
    }
    // visit 用法
    std::visit(visitor, anotherData);

    anotherData = 10; 
    value = std::get_if<int>(&anotherData);
    if(value != nullptr){
        std::cout << *value << std::endl;
    }else
    {
        std::cout << "Not Found" << std::endl;
    }
    // visit 用法
    std::visit(visitor, anotherData);

}
