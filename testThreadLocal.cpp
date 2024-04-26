#include <iostream>
#include <vector>
#include <thread>
#include <future>

void test_thread_local(int num){
    thread_local int i = num;
    std::cout << i << std::endl;
    i++;
}

int main()
{
    // async
    std::vector<std::future<void>> futures;

    for (int i = 0; i < 10; ++i) {
        futures.push_back(std::async(std::launch::async, [i]() {
            test_thread_local(i);
        }));
    }

    for (auto &f : futures) {
        f.wait();
    }
    
    return 0;
}