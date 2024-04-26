#include <iostream>

class Base {
  friend bool operator==(const Base& lhs, const Base& rhs);

 public:
  Base(std::string name = "Base") : m_base_name(name) {}
  virtual ~Base() = default;

  virtual void f() { std::cout << "Base::f()" << std::endl; }

  virtual bool equal(const Base& rhs) const {
    return m_base_name == rhs.m_base_name;
  }

  void test(const std::string& name) {
    m_base_name = name;
    std::cout << "Base Test()" << std::endl;
  }

 private:
  std::string m_base_name;
};

class Derived : public Base {
 public:
  Derived(std::string name = "Derived") : Base(name), m_derived_name(name) {}
  virtual ~Derived() = default;
  virtual void f() override { std::cout << "Derived::f()" << std::endl; }

  virtual bool equal(const Base& rhs) const override {
    const Derived& derived = dynamic_cast<const Derived&>(rhs);
    return m_derived_name == derived.m_derived_name;
  }

  void test(const std::string& name) {
    m_derived_name = name;
    std::cout << "Derived Test()" << std::endl;
  }

 private:
  std::string m_derived_name;
};

bool operator==(const Base& lhs, const Base& rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.equal(rhs);
}

void test_equal(){
  Base base, base1;
  Derived derived, derived1;


  Base* pBase = &base;
  Base* pBase1 = &base1;
  *pBase == *pBase1 ? std::cout << "base == base1" << std::endl
                : std::cout << "base != base1" << std::endl;
  base1.test("Base1");
  *pBase == *pBase1 ? std::cout << "base == base1" << std::endl
                : std::cout << "base != base1" << std::endl;

  Base* pBase2 = &derived;
  Base* pBase3 = &derived1;
  *pBase2 == *pBase3 ? std::cout << "derived == derived1" << std::endl
                : std::cout << "derived != derived1" << std::endl;
  derived1.test("Derived1");
  *pBase2 == *pBase3 ? std::cout << "derived == derived1" << std::endl
                : std::cout << "derived != derived1" << std::endl;

  std::cout << "------------------------------------" << std::endl;
}

void test_dynamic_cast(){
  Base base;
  Derived derived;

  Base* pBase = &base;
  Base* pBase1 = &derived;

  // test call virtual func from base pointer
  pBase->f();
  pBase1->f();
  std::cout << "------------------------------------" << std::endl;
  // test call non-virtual func from base pointer
  pBase->test("test");
  pBase1->test("test");

  if(Derived* pDerived = dynamic_cast<Derived*>(pBase1)){
    pDerived->test("test");
  }

  if(Derived* pDerived = dynamic_cast<Derived*>(&base)){
    std::cout << "base to derived success" << std::endl;
  }else{
    std::cout << "base to derived failed" << std::endl;
  }

  if(Base* pBase = dynamic_cast<Derived*>(&derived)){
    std::cout << "derived to base success" << std::endl;
  }else{
    std::cout << "derived base failed" << std::endl;
  }



  std::cout << "------------------------------------" << std::endl;
}

int main(int argc, const char** argv) {
    test_dynamic_cast();


  return 0;
}