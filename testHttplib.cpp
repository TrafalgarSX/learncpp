#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

void test_httplib()
{
    // HTTP
    httplib::Client cli("https://www.baidu.com");

    auto res = cli.Get("/");
    std::cout << "http code: " << res->status << std::endl;
    std::cout << "http body" << res->body << std::endl;
}

int main(int argc, const char** argv) {
    test_httplib();
    return 0;
}