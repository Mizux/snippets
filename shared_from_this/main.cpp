#include <memory>
#include <iostream>

struct Good : std::enable_shared_from_this<Good> {
  std::shared_ptr<Good> getptr() { return shared_from_this(); }
  ~Good() { std::cout << "Good::~Good() called\n"; }
};

struct Bad {
  std::shared_ptr<Bad> getptr() { return std::shared_ptr<Bad>(this); }
  ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};

struct Foo : std::enable_shared_from_this<Foo> {
  virtual ~Foo() { std::cout << "Foo::~Foo() called\n"; }
  virtual std::shared_ptr<Foo> getPtr() {
    std::cout << "Foo::getPtr() called\n";
    return shared_from_this();
  }
};

struct Bar : public Foo {
  virtual ~Bar() { std::cout << "Bar::~Bar() called\n"; }
  virtual std::shared_ptr<Foo> getPtr() {
    std::cout << "Bar::getPtr() called\n";
    return shared_from_this();
  }
};

int main() {
  {  // Good: the two shared_ptr's share the same object
    std::shared_ptr<Good> gp1(new Good);
    std::shared_ptr<Good> gp2 = gp1->getptr();
    std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';
  }

  {  // Bad, each shared_ptr thinks it's the only owner of the object
    std::shared_ptr<Bad> bp1(new Bad);
    std::shared_ptr<Bad> bp2 = bp1->getptr();
    std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
  }

  {
    std::shared_ptr<Foo> fooPtr = std::make_shared<Bar>();
    std::shared_ptr<Foo> foo2Ptr = fooPtr->getPtr();
    std::cout << "foo2Ptr.use_count() = " << foo2Ptr.use_count() << '\n';
  }
}  // UB: double-delete of Bad
