#include <iostream>
#include <functional>

#include <conio.h>

class Screen{
    friend void test_member_pointer_func();
public:
    typedef std::string::size_type pos;
    Screen() = default;
    char get_cursor() const { return contents[cursor]; };
    char get() const &&;
    char get() const &;
    static const std::string Screen::*string_data() { return &Screen::contents; }
private:
    std::string contents{"Hello, World!"};
    pos cursor;
    pos height, width;
    std::string test{"test"};
};

void test_member_pointer_func(){
    const std::string Screen::*pdata;
    const Screen::pos Screen::*ppos;

    pdata = &Screen::contents;
    auto pdata2 = &Screen::test;

    Screen myScreen, *pScreen = &myScreen;

    // 1. 解引用数据成员指针，以得到所需的成员 *
    // 2. 然后像成员访问运算符一样访问成员  . -> 
    // 3. 用 .* 或 ->* 运算符来访问成员  合起来就是这样
    auto s = myScreen.*pdata;
    std::cout << s << std::endl;
    s = pScreen->*pdata;
    std::cout << s << std::endl;

    auto test = myScreen.*pdata2;
    std::cout << test << std::endl;

    // 成员函数指针和普通函数指针不同的是，成员函数和指向成员函数的指针不存在自动转换规则
    auto member_func_pointer = &Screen::get_cursor;

    char (Screen::*mfpg)() const = &Screen::get_cursor;

    char ch = (myScreen.*mfpg)();
}

namespace mem_func_list{
    class Screen{
        public:
            using Action = Screen& (Screen::*)();
            Screen& home();
            Screen& forward();
            Screen& back();
            Screen& up();
            Screen& down();

            enum class Directions {HOME, FORWARD, BACK, UP, DOWN};
            Screen& move(Directions);
        private:
            static Action Menu[];
    };


    Screen& Screen::move(Directions cm){
        return (this->*Menu[static_cast<int>(cm)])();
    }
    Screen& Screen::home(){
        std::cout << "home" << std::endl;
        return *this;
    }

    Screen& Screen::forward(){
        // std::cout << "forward" << std::endl;
        std::cout << "forward ";
        return *this;
    }

    Screen& Screen::back(){
        // std::cout << "back" << std::endl;
        std::cout << "back ";
        return *this;
    }

    Screen& Screen::up(){
        // std::cout << "up" << std::endl;
        std::cout << "up ";
        return *this;
    }

    Screen& Screen::down(){
        // std::cout << "down" << std::endl;
        std::cout << "down ";
        return *this;
    }

    Screen::Action Screen::Menu[] = {
        &Screen::home,
        &Screen::forward,
        &Screen::back,
        &Screen::up,
        &Screen::down
    };
}

int main(int argc, const char** argv) {
    auto p = Screen::string_data();
    // test_member_pointer_func();

    using mem_func_list::Screen;
    Screen myScreen;

    unsigned char ch;
    Screen::Directions dir;
    while (true) {
        ch = _getch();
        if(ch == 'q') break;
        if (ch == 0 || ch == 224) {
            switch (_getch()) {
                case 72:  // Up arrow key
                    dir = Screen::Directions::UP;
                    break;
                case 80:  // Down arrow key
                    dir = Screen::Directions::DOWN;
                    break;
                case 75:  // Left arrow key
                    dir = Screen::Directions::BACK;
                    break;
                case 77:  // Right arrow key
                    dir = Screen::Directions::FORWARD;
                    break;
            }
        }
        myScreen.move(dir);
    }

    auto fcn = &std::string::empty;
    std::cout << "fcn type: " << typeid(fcn).name() << std::endl;

    auto mem_fc = std::mem_fn(&std::string::empty);
    std::cout << "fcn type: " << typeid(mem_fc).name() << std::endl;
}