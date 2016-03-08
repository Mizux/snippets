#include <memory>

namespace std {
template <typename T, typename Deleter = default_delete<T> >
class transitive_unique_ptr : public unique_ptr<T, Deleter> {
 public:
  // inherit typedefs for the sake of completeness
  using pointer = typename unique_ptr<T, Deleter>::pointer;
  using const_pointer = const typename remove_pointer<pointer>::type*;
  using element_type = typename unique_ptr<T, Deleter>::element_type;
  using deleter_type = typename unique_ptr<T, Deleter>::deleter_type;

  using unique_ptr<T, Deleter>::unique_ptr;

  // add transitively const version of get()
  pointer get() { return unique_ptr<T, Deleter>::get(); }
  const_pointer get() const { return unique_ptr<T, Deleter>::get(); }

  // add transitively const version of operator*()
  typename add_lvalue_reference<T>::type operator*() { return *get(); }
  typename add_lvalue_reference<const T>::type operator*() const {return *get(); }

  // add transitively const version of operator->()
  pointer operator->() { return get(); }
  const_pointer operator->() const { return get(); }
};

template<typename T> class transitive_weak_ptr;

template <typename T>
class transitive_shared_ptr: public shared_ptr<T> {
 public:
  using element_type = typename shared_ptr<T>::element_type;

  using shared_ptr<T>::shared_ptr;

  // add transitively const version of get()
  T* get() {return shared_ptr<T>::get();}
  const T* get() const {return shared_ptr<T>::get();}

  // add transitively const version of operator*()
  element_type& operator*() noexcept {return *get();}
  const element_type& operator*() const noexcept {return *get();}

  // add transitively const version of operator->()
  T* operator->() {return get();}
  const T* operator->() const {return get();}

  private:
  // This constructor is non-standard, it is used by weak_ptr::lock().
  transitive_shared_ptr(const transitive_weak_ptr<T>& r, nothrow_t)
  : shared_ptr<T>(r, nothrow) { }

  friend class transitive_weak_ptr<T>;
};

template <typename T>
class transitive_weak_ptr: public weak_ptr<T> {
 public:
  using element_type = typename weak_ptr<T>::element_type;

  using weak_ptr<T>::weak_ptr;

  // add transitively const version of lock()
  const transitive_shared_ptr<T> lock() const {return transitive_shared_ptr<T>(*this, nothrow);}
  transitive_shared_ptr<T> lock() {return transitive_shared_ptr<T>(*this, nothrow);}
};

struct Sp_make_transitive_shared_tag {};

template <typename T, typename Alloc, typename... Args>
inline transitive_shared_ptr<T> allocate_transitive_shared(const Alloc& a,
                                                           Args&&... args) {
  return transitive_shared_ptr<T>(Sp_make_transitive_shared_tag(), a,
                                  forward<Args>(args)...);
}

template <typename T, typename... Args>
inline transitive_shared_ptr<T> make_transitive_shared(Args&&... args) {
  typedef typename remove_const<T>::type T_nc;
  return allocate_transitive_shared<T>(allocator<T_nc>(),
                                            forward<Args>(args)...);
}

}

#include <vector>
#include <iostream>

// Small example where a Class (Foo) contain a polymorphe element (int*)
// How manage to get transitivity of const ?
// aka const Foo, imply const x and vice-verca...
struct Foo {
  int a;
};

struct Bar {
  Bar() : uniqueFoo(new Foo), uniqueConstFoo(new Foo), transitiveUniqueFoo(new Foo), transitiveSharedFoo(new Foo), transitiveWeakFoo(transitiveSharedFoo) {}
  std::unique_ptr<Foo> uniqueFoo;
  std::unique_ptr<const Foo> uniqueConstFoo;
  std::transitive_unique_ptr<Foo> transitiveUniqueFoo;

  std::transitive_shared_ptr<Foo> transitiveSharedFoo;
  std::transitive_weak_ptr<Foo> transitiveWeakFoo;

  void constMethod() const {
    uniqueFoo->a = 42;
    //uniqueConstFoo->a = 42; // uniqueConst is const...
    //transitiveUniqueFoo->a = 42; // GOOD method is const thus transitiveUnique is const !
    //transitiveSharedFoo->a = 42; // GOOD method is const thus transitiveUnique is const !
    //transitiveWeakFoo.lock()->a = 42; // GOOD method is const thus transitiveUnique is const !
  }
  void method() {
    uniqueFoo->a = 42;
    const_cast<Foo&>(*uniqueConstFoo).a = 42; // BAD need const_cast ugly
    transitiveUniqueFoo->a = 42; // GOOD method is not const thus transitiveUnique is not !
    transitiveSharedFoo->a = 42; // GOOD method is not const thus transitiveShared is not !
    transitiveWeakFoo.lock()->a = 42; // GOOD method is not const thus transitiveWeak is not !
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
