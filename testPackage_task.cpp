#include <iostream>
#include <thread>
#include <future>


std::string add_to_string(int a, int b) {
    return std::to_string(a+b);
}


int main(int argc, const char** argv) {
    std::packaged_task<std::string(int,int)> task(add_to_string); // 设置 packaged_task
    std::future<std::string> ret = task.get_future(); // 获得与 packaged_task 共享状态相关联的 future 对象.

    std::thread th(std::move(task), 10, 5);   //创建一个新线程完成计数任务.

    std::string value = ret.get();                    // 等待任务完成并获取结果.

    std::cout << "value is " << value << ".\n";

    th.join();
    return 0;
}