#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
 
struct A
{
    std::string s;
    A() = default;
 
    A(std::string str) : s(std::move(str)) { std::cout << "constructor\n"; }
 
    A(const A& o) : s(o.s) { std::cout << "copy constructor\n"; }
 
    A(A&& o) : s(std::move(o.s)) { std::cout << "move constructor\n"; }
 
    A& operator=(const A& other)
    {
        s = other.s;
        std::cout << "copy assign\n";
        return *this;
    }
 
    A& operator=(A&& other)
    {
        s = std::move(other.s);
        std::cout << "move assign\n";
        return *this;
    }
};

void vectorEmplace()
{
    std::vector<A> container;
    // 预留足够的空间以使 vector 不必重设大小
    container.reserve(6);

    std::cout << "构造 2 次A:\n";
    A two { "two" };
    A three { "three" };
 
    std::cout << "emplace:\n";
    container.emplace(container.end(), "one");
 
    std::cout << "以A& 调用 emplace:\n";
    container.emplace(container.end(), two);
 
    std::cout << "以A&& 调用 emplace:\n";
    container.emplace(container.end(), std::move(three));

    std::cout << "\n";

    std::cout << "push_back\n";
    std::string four = "four";
    container.push_back(four);
    std::cout << "以A& 调用 push_back:\n";
    container.push_back(two);
    std::cout << "以A&& 调用 push_back:\n";
    container.push_back(std::move(three));
 
    std::cout << "内容:\n";
    for (const auto& obj : container)
        std::cout << ' ' << obj.s;
    std::cout << '\n';
}

void mapEmplace()
{
    std::map<int, A> container;
    std::cout << "构造 2 次A:\n";
    A two { "two" };
    A three { "three" };
 
    std::cout << "emplace:\n";
    container.emplace(1, "one");
 
    std::cout << "以A& 调用 emplace:\n";
    container.emplace(std::make_pair(2, two));
 
    std::cout << "以A&& 调用 emplace:\n";
    container.emplace(std::make_pair(3, std::move(three)));

    std::cout << "\n";

    std::cout << "insert\n";
    std::string four = "four";
    container.insert(std::make_pair(4, four));
    std::cout << "以A& 调用 push_back:\n";
    container.insert(std::make_pair(5, two));

    container.insert(std::make_pair(6, std::move(three)));
    std::cout << "内容:\n";

    std::cout << '\n';
}

 
int main()
{
    // vectorEmplace();
    mapEmplace();
}