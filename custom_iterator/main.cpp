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

struct IFoo {
  virtual ~IFoo() { std::cout << "IFoo::~IFoo() called\n"; }
  virtual std::shared_ptr<IFoo> getPtr() = 0;
};

struct FooBase : std::enable_shared_from_this<FooBase>, IFoo {
  virtual ~FooBase() { std::cout << "FooBase::~FooBase() called\n"; }
  virtual std::shared_ptr<IFoo> getPtr() {
    std::cout << "FooBase::getPtr() called\n";
    return shared_from_this();
  }
};

struct FooDerived : public FooBase {
  virtual ~FooDerived() { std::cout << "FooDerived::~FooDerived() called\n"; }
  virtual std::shared_ptr<IFoo> getPtr() {
    std::cout << "FooDerived::getPtr() called\n";
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
  } // Ultra Bad: double-delete of Bad

  {
    std::shared_ptr<IFoo> fooPtr = std::make_shared<FooDerived>(); // FooDerived Instantiate...
    std::shared_ptr<IFoo> foo2Ptr = fooPtr->getPtr();
    std::shared_ptr<FooDerived> foo3Ptr = std::static_pointer_cast<FooDerived>(fooPtr->getPtr()); // BAD but WORK
    std::shared_ptr<FooDerived> foo4Ptr = std::dynamic_pointer_cast<FooDerived>(fooPtr->getPtr()); // OK
    std::cout << "foo2Ptr.use_count() = " << foo2Ptr.use_count() << '\n'; // 4
    std::cout << "foo3Ptr.use_count() = " << foo3Ptr.use_count() << '\n'; // 4
    std::cout << "foo4Ptr.use_count() = " << foo4Ptr.use_count() << '\n'; // 4
  }
  {
    std::shared_ptr<IFoo> fooPtr = std::make_shared<FooBase>(); // /!\ FooBase intantiate /!\ .
    std::shared_ptr<IFoo> foo2Ptr = fooPtr->getPtr();
    std::shared_ptr<FooDerived> foo3Ptr = std::static_pointer_cast<FooDerived>(fooPtr->getPtr()); // BAD could crash later on call
    std::shared_ptr<FooDerived> foo4Ptr = std::dynamic_pointer_cast<FooDerived>(fooPtr->getPtr()); // OK foo4Ptr will be empty
    std::cout << "foo2Ptr.use_count() = " << foo2Ptr.use_count() << '\n'; // 3
    std::cout << "foo3Ptr.use_count() = " << foo3Ptr.use_count() << '\n'; // 3
    std::cout << "foo4Ptr.use_count() = " << foo4Ptr.use_count() << '\n'; // 0
  }
}
