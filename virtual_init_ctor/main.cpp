#include <memory>
#include <iostream>

// Show that virtual method are not overloaded in constructor

class Base {
 public:
  Base() {
    init();
    std::cout << "Base::Base() called\n";
  }
  virtual ~Base() { std::cout << "Base::~Base() called\n"; }
  virtual void foo() { std::cout << "Base::foo() called\n"; }
  void bar() { std::cout << "Base::bar() called\n"; }

 protected:
  virtual void init() { std::cout << "Base::init() called\n"; }
};

class Derived : public Base {
 public:
  Derived() {
    init();
    std::cout << "Derived::Derived() called\n";
  }
  virtual ~Derived() { std::cout << "Derived::~Deriv() called\n"; }
  virtual void foo() { std::cout << "Derived::foo() called\n"; }
  void bar() { std::cout << "Derived::bar() called\n"; }

 protected:
  virtual void init() { std::cout << "Derived::init() called\n"; }
};

int main() {
  std::shared_ptr<Base> basePtr =
      std::make_shared<Derived>();  // call both init functions !

  basePtr->foo();                                      // call Derived method !
  std::dynamic_pointer_cast<Derived>(basePtr)->foo();  // call Derived Method

  basePtr->bar();                                      // call Base method .
  std::dynamic_pointer_cast<Derived>(basePtr)->bar();  // call Derived method
}
