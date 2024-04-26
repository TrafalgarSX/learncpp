#include <iostream>
#include <utility>
#include <string>
#include <fmt/core.h>

std::tuple<int, std::string, int> CreatePerson(){
    return {0, "Cherno", 24};
}

std::pair<std::string, int> CreatePersonAnother(){
    return {"guoaywen", 25};
}

int main(void){

    std::pair<std::string, int> person = CreatePersonAnother();
    std::string& name3 = person.first;
    int& age3 = person.second;

    fmt::print("Name: {}, Age: {}\n", name3, age3);

    std::string name2;
    int age2;
    std::tie(name2, age2) = CreatePersonAnother();
    fmt::print("Name: {}, Age: {}\n", name2, age2);

    auto [result, name, age] = CreatePerson();
   
    fmt::print("Result: {}\n", result);
    fmt::print("Name: {}, Age: {}\n", name, age);

    return 0;

}