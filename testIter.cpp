#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <string>
#include <utility>

void vectorIter()
{
    std::vector<std::pair<int, std::string>> v = {{1,"a"}, {2,"b"}, {3,"c"}, {4,"d"}, {5, "e"} };

    for (auto it = v.begin(); it != v.end(); )
    {
#if 1
        if(it->first == 2){
            it = v.erase(it);
        }else{
            ++it;
        }
#else
        // 这种写法是错误的，因为erase之后，it已经失效了，不能再使用了
        // 程序会崩溃
        if(it->first == 2){
            v.erase(it);
        }
        ++it;
#endif
    }

    for (auto it = v.begin(); it != v.end(); ++it)
    {
        std::cout << it->second << std::endl;
    }
}

void unordered_mapIter()
{
    std::unordered_map<int, std::string> m = {{1,"a"}, {2,"b"}, {3,"c"}, {4,"d"}, {5, "e"} };

    for (auto it = m.begin(); it != m.end(); )
    {
#if 1
    #if 0
        if(it->first == 2){
            it = m.erase(it);
        }else{
            ++it;
        }
    #else 
        // 这种方式也可以, 在it失效之前，传值，然后再++it
        if(it->first == 2){
            m.erase(it++);
        }else{
            ++it;
        }
    #endif
#else
        // 这种写法是错误的，因为erase之后，it已经失效了，不能再使用了
        // 程序会崩溃
        if(it->first == 2){
            m.erase(it);
        }
        ++it;
#endif
    }

    for (auto it = m.begin(); it != m.end(); ++it)
    {
        std::cout << it->second << std::endl;
    }
}

void mapIter()
{
    std::map<int, std::string> m = {{1,"a"}, {2,"b"}, {3,"c"}, {4,"d"}, {5, "e"} };

    for (auto it = m.begin(); it != m.end(); )
    {
#if 1
    #if 0
        if(it->first == 2){
            it = m.erase(it);
        }else{
            ++it;
        }
    #else 
        // 这种方式也可以, 在it失效之前，传值，然后再++it
        if(it->first == 2){
            m.erase(it++);
        }else{
            ++it;
        }
    #endif
#else
        // 这种写法是错误的，因为erase之后，it已经失效了，不能再使用了
        // 程序会崩溃
        if(it->first == 2){
            m.erase(it);
        }
        ++it;
#endif
    }

    for (auto it = m.begin(); it != m.end(); ++it)
    {
        std::cout << it->second << std::endl;
    }
}

void listIter()
{
    std::list<std::pair<int, std::string>> m = {{1,"a"}, {2,"b"}, {3,"c"}, {4,"d"}, {5, "e"} };

    for (auto it = m.begin(); it != m.end(); )
    {
#if 1
    #if 0
        if(it->first == 2){
            it = m.erase(it);
        }else{
            ++it;
        }
    #else 
        // 这种方式也可以, 在it失效之前，传值，然后再++it
        if(it->first == 2){
            m.erase(it++);
        }else{
            ++it;
        }
    #endif
#else
        // 这种写法是错误的，因为erase之后，it已经失效了，不能再使用了
        // 程序会崩溃
        if(it->first == 2){
            m.erase(it++);
        }
        ++it;
#endif
    }

    for (auto it = m.begin(); it != m.end(); ++it)
    {
        std::cout << it->second << std::endl;
    }

}

int main(){

    // vectorIter();
    unordered_mapIter();
    mapIter();
    listIter();
}