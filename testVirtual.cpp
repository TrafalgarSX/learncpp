#include <iostream>

class Base{
public:
    virtual int func(int x){
        std::cout << "base virtual func" << std::endl;
        return x;
    }

    virtual Base* func_return(){
        std::cout << "base virtual func" << std::endl;
        return this;
    }
};

class Derive: public Base{
public:
#if 1
    virtual int func(int x){
        std::cout << "derive virtual func" << std::endl;
        return x;
    }
#endif
    virtual Derive* func_return(){
        return this;
    }

#if 0
    // 会隐藏 virtual func
    void func(std::string){
        std::cout << "derive non-virtual func" << std::endl;
    }
#endif
    int num;
};

// class DD: private Derive{
class DD: public Derive{
public:
    DD(){

    }

private:
    static int b;
};

int DD::b = 10;

int main(int argc, const char** argv) {
    Derive d;

    Base* pb = &d;
    pb->func(5);

    DD dd;

    Base* pbb = &dd;
    pbb->func(2);
    return 0;

}


class Father{
public:
    int size() const{return n;}

protected:
    int n;
};

class Son: private Father{
public:
    using Father::size;
protected:
    using Father::n;
};