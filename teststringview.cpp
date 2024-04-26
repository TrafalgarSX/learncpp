#include <iostream>
#include <string>
#include <string_view>
#include <fmt/core.h>

void print(std::string_view sv)
{
    std::cout << sv << '\n';
}

void change(std::string& s)
{
    s[0] = 'X';
}

int main()
{
    std::string_view sv("Hello World");
    std::cout << sv << '\n';
    std::cout << sv.substr(6, 5) << '\n';
    std::cout << sv.substr(6) << '\n';

    std::string s("Hello World");
    change(s);
    std::cout << s << '\n';

}