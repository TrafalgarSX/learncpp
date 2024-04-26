#include <iostream>
#include <fstream>

void read_file()
{
    std::ifstream ifs("test.txt", std::ios::binary | std::ios::in);

    if (!ifs.is_open()) {
        std::cout << "open file failed" << std::endl;
        return;
    }

    char buf[1024] = {0};
    while (!ifs.eof()) {
        ifs.read(buf, 1024);
        std::cout << buf << std::endl;
    }

    ifs.close();
}

void write_file()
{
    std::ofstream ofs("test.txt", std::ios::binary | std::ios::out);

    if (!ofs.is_open()) {
        std::cout << "open file failed" << std::endl;
        return;
    }

    char buf[1024] = {0};
    while (std::cin.getline(buf, 1024)) {
        if(strcmp(buf, "quit") == 0) {
            break;
        }
        int len = std::cin.gcount();
        std::cout << "len: " << len << std::endl;
        ofs.write(buf, len);
        memset(buf, 0, len);
    }

    ofs.close();
}

int main(void){

    write_file();
    read_file();

    return 0;
}
