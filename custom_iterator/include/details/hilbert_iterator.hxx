#pragma once

#include <hilbert_iterator.hpp>
#include <cmath>

namespace hilbert {

template <typename T>
iterator<T>::iterator(Matrix<T>& mat, std::size_t pos)
	: _mat(mat), _pos(pos) {
	//std::cout << "hilbert::iterator::iterator() called\n";
}

template <typename T>
iterator<T>::~iterator() {
	//std::cout << "hilbert::iterator::~iterator() called\n";
}

template <typename T>
iterator<T> iterator<T>::begin() const {
	return iterator<T>(_mat);
}

template <typename T>
iterator<T> iterator<T>::end() const {
	return iterator<T>(_mat, _mat.size());
}

template <typename T>
bool iterator<T>::operator==(const iterator<T>& rhs) const {
  if (&_mat != &rhs._mat)
    throw std::runtime_error("iterator not on the same container");
  return _pos == rhs._pos;
}

template <typename T>
bool iterator<T>::operator!=(const iterator<T>& rhs) const {
  return !(*this == rhs);
}

template <typename T>
T& iterator<T>::operator*() {
  auto res = BinaryToXY(_pos);     // From _pos compute x, y
  return _mat.data[res.y][res.x];  // From x, y retrieve the value
}

template <typename T>
iterator<T>& iterator<T>::operator++() {  // prefix
  ++_pos;
  return *this;
}

template <typename T>
iterator<T> iterator<T>::operator++(int) {  // postfix
  iterator res(_mat, _pos);
  ++(*this);
  return res;
}

template <typename T>
iterator<T>& iterator<T>::operator--() {  // prefix
  --_pos;
  return *this;
}

template <typename T>
iterator<T> iterator<T>::operator--(int) {  // postfix
  iterator res(_mat, _pos);
  --(*this);
  return res;
}

//////////////////
//  CONVERTION  //
//////////////////

// rotate/flip a quadrant appropriately
void rot(std::size_t n,
         std::size_t& x,
         std::size_t& y,
         std::size_t rx,
         std::size_t ry) {
  if (ry == 0) {
    if (rx == 1) {
      x = n - 1 - x;
      y = n - 1 - y;
    }
    // Swap x and y
    std::size_t t = x;
    x = y;
    y = t;
  }
}

template <typename T>
std::size_t iterator<T>::XYToBinary(Vector<std::size_t> vec) const {
  if (vec.x < 0 || vec.x >= _mat.n || vec.y < 0 || vec.y >= _mat.n)
    throw std::runtime_error("X,Y must be in [0;n-1]");

  std::size_t rx, ry, d = 0;
  for (std::size_t s = _mat.n / 2; s > 0; s /= 2) {
    rx = (vec.x & s) > 0;
    ry = (vec.y & s) > 0;
    d += s * s * ((3 * rx) ^ ry);
    rot(s, vec.x, vec.y, rx, ry);
  }
  return d;
}

template <typename T>
Vector<std::size_t> iterator<T>::BinaryToXY(std::size_t d) const {
  if (d < 0 || d > _mat.size() - 1)
    throw std::range_error("Distance must be in range [0, n^2-1]");

  Vector<std::size_t> out(0, 0);
  std::size_t rx, ry, s, t = d;
  for (s = 1; s < _mat.n; s *= 2) {
    rx = 1 & (t / 2);
    ry = 1 & (t ^ rx);
    rot(s, out.x, out.y, rx, ry);
    out.x += s * rx;
    out.y += s * ry;
    t /= 4;
  }
  return out;
}

}
