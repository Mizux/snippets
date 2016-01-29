#pragma once

#include <vector>

//! @class Matrix container (2D Vector).
template <typename T>
struct Vector {
  Vector(T x, T y) : x(x), y(y) {}
  ~Vector() {}
  T x;
  T y;
};

//! @class Matrix container (2D Matrix).
template <typename T>
struct Matrix {
  //! Create a container of n^2 elements aka matrix of n x n elments.
  //! @param[in] n must be a power of two.
  //! @param[in] value value use to initialize the matrix.
  Matrix(std::size_t n, const T& value = 0);
  ~Matrix();

  //! @{
  class const_iterator {
   public:
    const_iterator(const Matrix& mat, std::size_t pos = 0);

    bool operator==(const const_iterator& rhs);
    bool operator!=(const const_iterator& rhs);
    const T& operator*();
    const_iterator& operator++();    // prefix
    const_iterator operator++(int);  // postfix
    const_iterator& operator--();    // prefix
    const_iterator operator--(int);  // postfix
   private:
    const Matrix& _mat;
    std::size_t _pos;
  };

  const_iterator begin() const;
  const_iterator end() const;
  //! @}

  //! @{
  class iterator {
   public:
    iterator(Matrix& mat, std::size_t pos = 0);

    bool operator==(const iterator& rhs);
    bool operator!=(const iterator& rhs);
    T& operator*();
    iterator& operator++();    // prefix
    iterator operator++(int);  // postfix
    iterator& operator--();    // prefix
    iterator operator--(int);  // postfix
   private:
    Matrix& _mat;
    std::size_t _pos;
  };

  iterator begin();
  iterator end();
  //! @}

  const std::size_t n;
  std::vector<std::vector<T>> data;  // row major

  std::size_t size() const;
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<T>& mat);

#include "details/matrix.hxx"
