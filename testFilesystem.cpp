#include <iostream>
#include <fstream>
#include <filesystem>

// alias std::filesystem 
namespace stdfs = std::filesystem;

// create file
void test_std_filesystem() {
    stdfs::path p = "test.txt";
    if(stdfs::exists(p)) {
        std::cout << "File exists" << std::endl;
    } else {
        std::cout << "File does not exist" << std::endl;
    }

    std::fstream file(p, std::ios::out);
    if(file.is_open()) {
        std::cout << "File created" << std::endl;
    } else {
        std::cout << "File not created" << std::endl;
    }

    file.close();

    stdfs::path current_path = stdfs::current_path();
    std::cout << "Current path: " << current_path << std::endl;
    std::cout << "root path: " << current_path.root_path() << std::endl;
    std::cout << "root name: " << current_path.root_name() << std::endl;
    std::cout << "root directory: " << current_path.root_directory() << std::endl;
    std::cout << "relative path: " << current_path.relative_path() << std::endl;

    stdfs::rename(p, "testfs.txt");

    stdfs::remove("testfs.txt");

    stdfs::path tmp = stdfs::temp_directory_path();
    std::cout << "Temp directory: " << std::quoted(tmp.string()) << std::endl;

    stdfs::path home = p / "home";


    std::cout << "std::quote: " << std::quoted("hello") << std::endl;

}



int main(int argc, const char** argv) {
    test_std_filesystem();
    return 0;
}