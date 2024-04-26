#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <map>
#include <unordered_map>
#include <numeric>
#include <fmt/core.h>
#include <Timer.hpp>

std::vector<int> indexs(10000);

void init_indexs()
{
    std::iota(indexs.begin(), indexs.end(), 0); // Fill with 0, 1, ..., 9999

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(indexs.begin(), indexs.end(), g); // Shuffle the numbers
}


class DevHandle{
public:
    DevHandle() = default;
    DevHandle(const DevHandle&) = default;
    DevHandle& operator=(const DevHandle&) = default;
    DevHandle(DevHandle&&) = default;
    DevHandle& operator=(DevHandle&&) = default;
    ~DevHandle() = default;

    bool operator<(const DevHandle& rhs) const
    {
        return index < rhs.index;
    }

    int index;
    void* ptr;
};

class DevHandleVector{
public:
    DevHandleVector() = default;
    DevHandleVector(const DevHandleVector&) = delete;
    DevHandleVector& operator=(const DevHandleVector&) = delete;
    DevHandleVector(DevHandleVector&&) = default;
    DevHandleVector& operator=(DevHandleVector&&) = default;
    ~DevHandleVector() = default;

    DevHandle& searchByIndex(int index)
    {
        // 二分法
        auto it = std::lower_bound(handles.begin(), handles.end(), DevHandle{index, nullptr});
        if (it != handles.end() && it->index == index)
        {
            return *it;
        }
        throw std::out_of_range("No handle with this index");
    }

    void insertSorted(DevHandle&& handle)
    {
#if 1
        auto it = std::lower_bound(handles.begin(), handles.end(), handle);
        handles.insert(it, std::move(handle));
#else
       // very slow
       handles.push_back(handle);
       std::sort(handles.begin(), handles.end(), [](const DevHandle& lhs, const DevHandle& rhs){
           return lhs.index < rhs.index;
       });
#endif
    }

    std::vector<DevHandle>::iterator deleteByIndex(int index)
    {
        auto it = std::lower_bound(handles.begin(), handles.end(), DevHandle{index, nullptr});
        if (it != handles.end() && it->index == index)
        {
            return handles.erase(it);
        }
        throw std::out_of_range("No handle with this index");
    }

    std::vector<DevHandle> handles;
};

int generate_random(int scope)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 20000); // Define the range
    return distrib(gen) % scope;
}

void test_vector()
{
    DevHandleVector handles;
    handles.handles.reserve(10000);
    Timer timer;

    {
        Timer timer;
        for (auto &index : indexs)
        {
            handles.insertSorted(DevHandle{index, nullptr});
        }
    }


    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            int result = handles.searchByIndex(indexs[i]).index;
            // fmt::print("search index: {}\n", result);
        }
    }

    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            if(handles.deleteByIndex(indexs[i]) != handles.handles.end())
            {
                // fmt::print("delete index: {}\n", indexs[i]);
            }
        }
    }
}

void test_list()
{
    std::list<DevHandle> handles;
    int nums = 10000;
    Timer timer;

    {
        Timer timer;
        for (auto &index : indexs)
        {
            handles.push_back(DevHandle{index, nullptr});
        }
    }

    // search
    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            int rand = generate_random(10000);
            for (auto &handle : handles)
            {
                if (handle.index == indexs[rand])
                {
                    // fmt::print("search index: {}\n", handle.index);
                    break;
                }
            }
        }
    }

    // delete
    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            int rand = generate_random(10000);
            for (auto it = handles.begin(); it != handles.end(); it++)
            {
                if (it->index == indexs[rand])
                {
                    // fmt::print("delete index: {}\n", it->index);
                    handles.erase(it);
                    break;
                }
            }
        }

    }
}

void test_unorderedmap()
{
    std::unordered_map<int, DevHandle> handles;
    int nums = 10000;
    Timer timer;

    {
        Timer timer;
        for (auto &index : indexs)
        {
            handles[index] = DevHandle{index, nullptr};
        }
        
    }

    // search
    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            int rand = generate_random(10000);
            auto it = handles.find(indexs[rand]);
            if (it != handles.end())
            {
                // fmt::print("search index: {}\n", it->second.index);
            }
        }
    }

    // delete
    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            int rand = generate_random(10000);
            auto it = handles.find(indexs[rand]);
            if (it != handles.end())
            {
                // fmt::print("delete index: {}\n", it->second.index);
                handles.erase(it);
            }
        }

    }
}

void test_map()
{
    std::map<int, DevHandle> handles;
    int nums = 10000;
    Timer timer;

    {
        Timer timer;
        for (auto &index : indexs)
        {
            handles[index] = DevHandle{index, nullptr};
        }
        
    }

    // search
    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            int rand = generate_random(10000);
            auto it = handles.find(indexs[rand]);
            if (it != handles.end())
            {
                // fmt::print("search index: {}\n", it->second.index);
            }
        }
    }

    // delete
    {
        Timer timer;
        for (int i = 0; i < 1000; i++)
        {
            int rand = generate_random(10000);
            auto it = handles.find(indexs[rand]);
            if (it != handles.end())
            {
                // fmt::print("delete index: {}\n", it->second.index);
                handles.erase(it);
            }
        }

    }
}

class Output{
public:
    std::fstream file;

    Output(){
        file.open("d:/output.txt", std::ios::out);
        file << "hello world\n";
    }

    ~Output(){
        file.close();
    }

    void write(const std::string_view& str){
        file << str << 10.03 << "\n";
    }
};

uint32_t __stdcall test_func(void* first, uint32_t second){
    return 0;
}

uint32_t test_func_cdecl(void* first, uint32_t second)
{
    return 0;
}


struct B {
    int x;
    int operator()() {
        return ++x;
    }
};


int main()
{
#if 0
    init_indexs();

    test_vector();
    std::cout << "-------------------------\n";
    test_list();
    std::cout << "-------------------------\n";
    test_unorderedmap();
    std::cout << "-------------------------\n";
    test_map();
    std::cout << "-------------------------\n";
#endif
    auto a = [] {
        int x = 0;
    #if 1
        auto b = [x]() mutable {
            return ++x;
        };
        return b;
    #else
        return B{x};
    #endif
    };
    auto b = a();
    auto c = a();
    std::cout<<b()<<std::endl;
    std::cout<<b()<<std::endl;

    std::cout<<c()<<std::endl;
    return 0;
}