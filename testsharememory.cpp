#include <windows.h>
#include <iostream>
#include <string>

int main() {
    // 创建一个信号量
    HANDLE hSemaphore = CreateSemaphore(
        NULL,  // 默认安全属性
        1,     // 初始计数
        1,     // 最大计数
        TEXT("guoyawen")); // 无名信号量

    if (hSemaphore == NULL) {
        std::cerr << "Could not create semaphore: " << GetLastError() << std::endl;
        return 1;
    }

    // 创建一个文件映射对象
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // 使用系统分页文件
        NULL,                    // 默认安全级别
        PAGE_READWRITE,          // 可读写访问
        0,                       // 最大对象大小（高位）
        256,                     // 最大对象大小（低位）
        TEXT("guoyawenShare"));   // 名称

    if (hMapFile == NULL) {
        std::cerr << "Could not create file mapping object: " << GetLastError() << std::endl;
        return 1;
    }

    // 映射视图到进程的地址空间
    char* pBuf = (char*) MapViewOfFile(
        hMapFile,                // 文件映射对象句柄
        FILE_MAP_ALL_ACCESS,     // 可读写访问
        0,                       // 高位偏移
        0,                       // 低位偏移
        256);                    // 映射的大小

    if (pBuf == NULL) {
        std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    while(true){
        // 获取信号量
        std::string info;
        std::cout << "Please input the data to be written to shared memory: ";
        std::cin >> info;

        WaitForSingleObject(hSemaphore, INFINITE);   
        // 写入数据到共享内存
        memcpy(pBuf, info.c_str(), 13);
        // 读取数据从共享内存
        std::cout << "Data read from shared memory: " << pBuf << std::endl;
        ReleaseSemaphore(hSemaphore, 1, NULL);

        Sleep(3000);
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(hSemaphore);
    return 0;
}