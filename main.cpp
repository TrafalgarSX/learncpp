#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base constructor" << std::endl;
    }
    virtual ~Base() {
        std::cout << "Base destructor" << std::endl;
    }

    virtual void print(){
        std::cout << "Base print" << std::endl;
    };
    void test();

public:
    long long data1;
    long long data2;
};

class AnotherBase{
public:
    AnotherBase() {
        std::cout << "AnotherBase constructor" << std::endl;
    }
    virtual ~AnotherBase() {
        std::cout << "AnotherBase destructor" << std::endl;
    }

    virtual void print(){
        std::cout << "AnotherBase print" << std::endl;
    };
    void test();
};

class Derive : public Base, public AnotherBase {

public:
    Derive() {
        std::cout << "Derive constructor" << std::endl;
    }
    ~Derive() {
        std::cout << "Derive destructor" << std::endl;
    }

    void print() override{};
    void test();
};

int main()
{
  char string[] = "hello world";

    char* charValue = new char(42);
    int* intValue = reinterpret_cast<int*>(&charValue[13]);

    // char* charValue = static_cast<char*>(static_cast<void*>(intValue));


  printf("test address is :%p\n", intValue);
  printf("string[2] address is :%p\n", &charValue[13]);

  Derive* derive = new Derive();
  derive->print();
  Base* base = derive;
  Derive* derive2 = static_cast<Derive*>(base);

  AnotherBase* anotherBase2 = static_cast<AnotherBase*>(derive);

  printf("derive address is :%p\n", derive);
  printf("derive2 address is:%p\n", derive2);
  printf("base address is   :%p\n", base);
  printf("anotherbase2 address is   :%p\n", anotherBase2);

  std::cout << "--------------------" << std::endl;

  Base* base2 = new Base();
  derive2 = static_cast<Derive*>(base2);
  Derive* derive3 = (Derive*)(base2);
  printf("base address is    :%p\n", base2);
  printf("derive2 address is :%p\n", derive2);
  printf("derive3 address is :%p\n", derive3);

  Derive* derive4 = dynamic_cast<Derive*>(base2);
  derive4 == nullptr ? std::cout << "derive4 is nullptr" << std::endl : std::cout << "derive4 is not nullptr" << std::endl;

  std::cout << "--------------------" << std::endl;
  AnotherBase* anotherBase = new AnotherBase();
  Derive* derive5 = (Derive*)(anotherBase);
  Derive* derive6 = static_cast<Derive*>(anotherBase);

  printf("anotherBase  address is :%p\n", anotherBase);
  printf("derive5 address is      :%p\n", derive5);
  printf("derive6 address is      :%p\n", derive6);

  printf("cast back address is    :%p\n", (AnotherBase*)derive6);

  Derive* derive7 = dynamic_cast<Derive*>(anotherBase);
  derive4 == nullptr ? std::cout << "derive7 is nullptr" << std::endl : std::cout << "derive7 is not nullptr" << std::endl;

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
