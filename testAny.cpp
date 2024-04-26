#include <iostream>
#include <string>
#include <any>

int main(void)
{
    std::any data;
    data = 2;
    data = "guoyawen";
    data = std::string("guoyawen");

    std::string &name = std::any_cast<std::string&>(data);
}