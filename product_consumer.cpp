#include <iostream>
#include <deque>
#include <thread>
#include <vector>
#include <Semaphore.hpp>
#include <Timer.hpp>
#include <ProAndCon.hpp>

ProAndCon<int> queue(100);

void consumer()
{
    
}

void producer()
{
}

int main()
{
    int num_consumer = 5;
    int num_producer = 5;

    Timer timer;

    std::vector<std::thread> consumer_threads;
    std::vector<std::thread> producer_threads;

    for (int i = 0; i < num_consumer; ++i) {
        consumer_threads.push_back(std::thread(consumer));
    }

    for (int i = 0; i < num_producer; ++i) {
        producer_threads.push_back(std::thread(producer));
    }

    for(auto &t : consumer_threads)
        t.join();

    for(auto &t : producer_threads)
        t.join();

}