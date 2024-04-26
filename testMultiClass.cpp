#include <iostream>
#include <assert.h>

class Father{
public:

    Father(){
        m_father = new int(1);
    }

    Father(int i){
        m_father = new int(i);
    }

    virtual ~Father(){
        delete m_father;
        std::cout << "Father destructor" << std::endl;
    }

    Father(const Father& father){
        m_father = new int(*father.m_father);
        std::cout << "Father copy constructor" << std::endl;
    }

    Father(Father&& father){
        m_father = father.m_father;
        father.m_father = nullptr;
        std::cout << "Father move constructor" << std::endl;
    }

    Father& operator=(Father father){
        using std::swap;
        swap(m_father, father.m_father);
        return *this;
    }

    void f(){
        std::cout << "Father f()" << std::endl;

    }

    int* m_father{nullptr};
};

class Mother{
public:
    Mother(){
        m_mother = new int(2);
    }

    Mother(int i){
        m_mother = new int(i);
    }

    virtual ~Mother(){
        delete m_mother;
        std::cout << "Mother destructor" << std::endl;
    }

    Mother(const Mother& mother){
        m_mother = new int(*mother.m_mother);
        std::cout << "Mother copy constructor" << std::endl;
    }

    Mother(Mother&& mother){
        m_mother = mother.m_mother;
        mother.m_mother = nullptr;
        std::cout << "Mother move constructor" << std::endl;
    }

    Mother& operator=(Mother mother){
        using std::swap;
        swap(m_mother, mother.m_mother);
        return *this;
    }

    void f(const std::string& str){
        std::cout << "Mother f(): "<< str << std::endl;
    }
    int* m_mother{nullptr};
};

class Son : public Father, public Mother{
public:
    Son(){
        std::cout << "Son constructor" << std::endl;
    }

    Son(int i, int j): Father(i), Mother(j){
        std::cout << "Son constructor i, j" << std::endl;
    }

    virtual ~Son(){
        std::cout << "Son destructor" << std::endl;
    }

    Son(const Son& son): Father(son), Mother(son){
        std::cout << "Son copy constructor" << std::endl;
    }

    Son(Son&& son): Father(std::move(son)), Mother(std::move(son)){
        std::cout << "Son move constructor" << std::endl;
    }

    friend void swap(Son& first, Son& second){
        using std::swap;
        swap(first.m_father, second.m_father);
        swap(first.m_mother, second.m_mother);
    }

    Son& operator=(Son son){
        using std::swap;
        swap(*this, son);
        return *this;
    }
};

int main(int argc, const char** argv) {
    Son son;
#if 0
    son.f();   // 二义性
#else
#endif
    Father* pf;
    Son* ps;

    pf = ps;
    pf = static_cast<Father*>(ps);
    pf = dynamic_cast<Father*>(ps);

    // ps = pf; // error  cannot convert from 'Father *' to 'Son *'
    ps = dynamic_cast<Son*>(pf);


    Son second_son(4, 5);
    std::cout << "-----------------" << std::endl;

    son = second_son;
    assert(son.m_father != second_son.m_father);
    assert(son.m_mother != second_son.m_mother);
    assert(*son.m_father == *second_son.m_father);
    assert(*son.m_mother == *second_son.m_mother);
    std::cout << *second_son.m_father << *second_son.m_mother << std::endl;
    std::cout << "-----------------" << std::endl;

    Son third_son = second_son;
    assert(third_son.m_father != second_son.m_father);
    assert(third_son.m_mother != second_son.m_mother);
    assert(*third_son.m_father == *second_son.m_father);
    assert(*third_son.m_mother == *second_son.m_mother);
    std::cout << "-----------------" << std::endl;

    int* temp[4];
    temp[0] = second_son.m_father;
    temp[1] = second_son.m_mother;
    Son fourth_son = std::move(second_son);
    assert(second_son.m_father == nullptr);
    assert(second_son.m_mother == nullptr);
    assert(fourth_son.m_father == temp[0]);
    assert(fourth_son.m_mother == temp[1]);
    std::cout << "-----------------" << std::endl;

    Son fifth_son;
    temp[0] = fourth_son.m_father;
    temp[1] = fourth_son.m_mother;
    fifth_son = std::move(fourth_son);
    assert(fourth_son.m_father == nullptr);
    assert(fourth_son.m_mother == nullptr);
    assert(fifth_son.m_father == temp[0]);
    assert(fifth_son.m_mother == temp[1]);
    std::cout << "-----------------" << std::endl;

    return 0;
}