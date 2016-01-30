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
  Foo() : w(new int), x(new int), y(new int), z(new int) {}
  int* w;
  int const* x;

  std::unique_ptr<int> y;
  std::transitive_ptr<int> z;
};

int main() {
  Foo foo;
  *foo.w = 1;
  //*foo.x = 1; // Bad compile fail !
  *foo.y = 1;
  *foo.z = 1;

  const Foo bar;
  *bar.w = 1;  // Bad no transitivity of const
  //*foo._y = 1; // OK compile fail !
  *bar.y = 1;  // Bad no transitivity of const
  //*bar.z = 1; // OK compile fail !
}

//  // Big Five
//  Bad(const Bad&) = default;
//  Bad& operator=(const Bad&) = default;
//  Bad(Bad&&) = default;
//  Bad& operator=(Bad&&) = default;
//  virtual ~Bad() = default;
