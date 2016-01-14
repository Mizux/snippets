#pragma once

#include "matrix.hpp"

namespace hilbert {
//! @class iterator_hilbert 2D hilbert curve iterator.
template <typename T>
class iterator {
 public:
  iterator(Matrix<T>& mat, std::size_t pos = 0);
  ~iterator();

  iterator<T> begin() const;
  iterator<T> end() const;

  bool operator==(const iterator& rhs) const;
  bool operator!=(const iterator& rhs) const;
  T& operator*();
  iterator& operator++();    // prefix
  iterator& operator--();    // prefix
  iterator operator++(int);  // postfix
  iterator operator--(int);  // postfix

  std::size_t XYToBinary(Vector<std::size_t> vec) const;
  Vector<std::size_t> BinaryToXY(std::size_t d) const;

 private:
  Matrix<T>& _mat;
  std::size_t _pos;
};

}

#include "details/hilbert_iterator.hxx"
