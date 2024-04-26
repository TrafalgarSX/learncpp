#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <fmt/core.h>

std::optional<std::string> ReadFileAsString(const std::string& filepath)
{
    std::fstream stream(filepath);
    if(stream){
        std::string result;
        // readfile 
        stream >> result;
        return result;
    }

    // return  {};
    return  std::nullopt;
}

int main(void)
{
    std::optional<std::string> data = ReadFileAsString("test.txt");

    fmt::print("{}\n", data.value_or("Not Found"));
    if(data.has_value()){
    // if(data){
        fmt::print("there has value {}\n", data.value());
    }else{
        fmt::print("Could not open file\n");
    }

    // 1.0 采用 std::nullopt 初始化再调用 emplace 插入值
    std::optional<int> val0 = std::nullopt;
    val0.emplace(128);
    fmt::print("val0 has value {}\n", val0.has_value());

    // 1.1 采用 {} 初始化再调用 emplace 插入值
    std::optional<int> val1 = {};
    val1.emplace(128);
    fmt::print("val1 has value {}\n", val1.has_value());

    // 1.2 采用 std::optional<T>(val) 初始化
    std::optional<int> val2 = std::optional<int>(128);
    fmt::print("val2 has value {}\n", val2.has_value());

    // 1.3 采用 std::optional(val) 初始化，自动推导变量类型
    std::optional<int> val3 = std::optional(128);
    fmt::print("val3 has value {}\n", val3.has_value());

    // 1.4 采用 std::optional<T>{val} 初始化
    std::optional<int> val4 = std::optional<int>{128};
    fmt::print("val4 has value {}\n", val4.has_value());

    // 1.5 采用 std::optional{val} 初始化
    std::optional<int> val5 = std::optional{128};
    fmt::print("val5 has value {}\n", val5.has_value());

    // 1.6 采用 {val} 初始化
    std::optional<int> val6 = {128};
    fmt::print("val6 has value {}\n", val6.has_value());

    // 1.7 采用 std::make_optional<T>(val) 初始化
    std::optional<int> val7 = std::make_optional<int>(128);
    fmt::print("val7 has value {}\n", val7.has_value());

    // 1.8 采用 std::make_optional(val) 初始化，自动推导变量类型
    std::optional<int> val8 = std::make_optional(128);
    fmt::print("val8 has value {}\n", val8.has_value());


    std::optional<int> result1 = 1;

    // 判断是否有值
    if (result1.has_value()) {
        std::cout << result1.value() << std::endl;
    }

    if (result1 != std::nullopt) {
        std::cout << result1.value() << std::endl;
    }

    if (result1) {
        std::cout << result1.value() << std::endl;
    }
    // 获取值的两种方式
    if (result1) {
        std::cout << result1.value() << std::endl;
    }
    if (result1) {
        std::cout << *result1 << std::endl;
    }



}