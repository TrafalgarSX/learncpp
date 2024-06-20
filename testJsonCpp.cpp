#include <iostream>
#include <json/json.h>

int main() {

    Json::Value root;

    root["name"] = "guoyawen";
    root["age"] = 25;
    root["isAlive"] = true;
    root["address"]["city"] = "New York";
    root["address"]["state"] = "NY";
    root["phoneNumbers"][0] = "212 555-1234";
    root["phoneNumbers"][1] = "646 555-4567";

    root["test"] = R"(c:\test\test.txt)";

    root["a"]["b"]["c"] = "test";


    // 创建一个Json::StreamWriterBuilder对象，用于将JSON对象转换为字符串
    Json::StreamWriterBuilder writer;
    std::string output = Json::writeString(writer, root);

    // 输出JSON字符串
    std::cout << output << std::endl;

    Json::Value root2;
    Json::CharReaderBuilder reader;
    std::string errs;
    std::istringstream is(output);

    if (!Json::parseFromStream(reader, is, &root2, &errs)) {
        std::cout << "Error parsing JSON: " << errs << std::endl;
        return 1;
    }

    std::cout << "City: " << root2["phoneNumbers"][0].asString() << std::endl;

    std::cout << "test: " << root2["test"] << std::endl;

    return 0;
}