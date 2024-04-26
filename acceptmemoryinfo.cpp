#include <windows.h>
#include <iostream>

int main() {
    // 打开一个已经存在的信号量
    HANDLE hSemaphore = OpenSemaphore(
        SEMAPHORE_ALL_ACCESS,  // 请求所有访问权限
        FALSE,                 // 不继承句柄
        TEXT("guoyawen")); // 信号量名称

    if (hSemaphore == NULL) {
        std::cerr << "Could not open semaphore: " << GetLastError() << std::endl;
        return 1;
    }
    // 打开一个已经存在的文件映射对象
    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // 可读写访问
        FALSE,                 // 不继承句柄
        TEXT("guoyawenShare")); // 名称

    if (hMapFile == NULL) {
        std::cerr << "Could not open file mapping object: " << GetLastError() << std::endl;
        return 1;
    }

    // 映射视图到进程的地址空间
    char* pBuf = (char*) MapViewOfFile(
        hMapFile,            // 文件映射对象句柄
        FILE_MAP_ALL_ACCESS, // 可读写访问
        0,                   // 高位偏移
        0,                   // 低位偏移
        256);                // 映射的大小

    if (pBuf == NULL) {
        std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    while(true){
        std::cout << "wait....\n";
        WaitForSingleObject(hSemaphore, INFINITE);
        // 读取数据从共享内存
        std::cout << "Data read from shared memory: " << pBuf << std::endl;
        if(strcmp(pBuf, "exit") == 0){
            break;
        }

        memset (pBuf, 0, 256);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        Sleep(500);
    }
    // 清理
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(hSemaphore);

    return 0;
}