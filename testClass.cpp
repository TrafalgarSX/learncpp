#include <iostream>

class Incomplete;

class Test{

    // class Incomplete;
    Incomplete test_func(Incomplete a);
private:
    Incomplete& r_obj;
    Incomplete* p_obj;
    // Incomplete obj; // error: field 'obj' has incomplete type 'Incomplete'

};

std::string str = "global field str";

int main(int argc, const char** argv) {
}