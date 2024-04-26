#include <iostream>
#include <chrono>
#include <thread>
#include <fmt/core.h>

struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        float ms = duration.count() * 1000.0f;
        fmt::print("Timer took {} ms\n", ms);
    }
};

void Function()
{
    Timer timer;

    for (int i = 0; i < 100; i++)
        fmt::print("Hello\n");
}

int main() {
#if 0
    auto start = std::chrono::high_resolution_clock::now();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    fmt::print("Elapsed time: {}\n", elapsed.count());
#else

    Function();
#endif
    return 0;
}