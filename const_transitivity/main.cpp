#include <vector>
#include <memory>
#include <iostream>

// Small example where a Class (Foo) contain a polymorphe element (int*)
// How manage to get transitivity of const ?
// aka const Foo, imply const x and vice-verca...

namespace std {
template <class T, class Deleter = std::default_delete<T> >
class transitive_ptr : public std::unique_ptr<T, Deleter> {
 public:
  // inherit typedefs for the sake of completeness
  typedef typename std::unique_ptr<T, Deleter>::pointer pointer;
  typedef typename std::unique_ptr<T, Deleter>::element_type element_type;
  typedef typename std::unique_ptr<T, Deleter>::deleter_type deleter_type;

  // extra typedef
  typedef const typename std::remove_pointer<pointer>::type* const_pointer;

  // inherit std::unique_ptr's constructors
  using std::unique_ptr<T, Deleter>::unique_ptr;

  // add transitively const version of get()
  pointer get() { return std::unique_ptr<T, Deleter>::get(); }
  const_pointer get() const { return std::unique_ptr<T, Deleter>::get(); }

  // add transitively const version of operator*()
  typename std::add_lvalue_reference<T>::type operator*() { return *get(); }
  typename std::add_lvalue_reference<const T>::type operator*() const {
    return *get();
  }

  // add transitively const version of operator->()
  pointer operator->() { return get(); }
  const_pointer operator->() const { return get(); }
};
}

struct Foo {
  int a;
};

struct Bar {
  Bar() : x(new Foo), y(new Foo), z(new Foo) {}
  std::unique_ptr<Foo> x;
  std::unique_ptr<const Foo> y;
  std::transitive_ptr<Foo> z;

  void constMethod() const {
    x->a = 42;
    //y->a = 42; // y is const...
    //z->a = 42; // GOOD method is const thus z is const !
  }
  void method() {
    x->a = 42;
    const_cast<Foo&>(*y).a = 42; // BAD need const_cast ugly
    z->a = 42; // GOOD method is not const thus z is not !
  }
};

int main() {
  Bar a;
  a.constMethod();
  a.method();

  const Bar b;
  b.constMethod();
  //b.method(); // b is const !
}

//  // Big Five
//  Foo(const Foo&) = default;  // cpy ctor
//  Foo& operator=(const Foo&) = default; // cpy assignement
//  Foo(Foo&&) = default; // move ctor
//  Foo& operator=(Foo&&) = default; // move assignement
//  virtual ~Foo() = default; // dtor
