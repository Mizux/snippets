#pragma once

#include <cmath>
#include <graycode_iterator.hpp>

namespace graycode {

template <typename T>
iterator<T>::iterator(Matrix<T>& mat, std::size_t pos)
  : _mat(mat)
  , _pos(pos) {
	// std::cout << "graycode::iterator::iterator() called\n";
}

template <typename T>
iterator<T>::~iterator() {
	// std::cout << "graycode::iterator::~iterator() called\n";
}

template <typename T>
iterator<T>
iterator<T>::begin() const {
	return iterator<T>(_mat);
}

template <typename T>
iterator<T>
iterator<T>::end() const {
	return iterator<T>(_mat, _mat.size());
}

template <typename T>
bool
iterator<T>::operator==(const iterator<T>& rhs) const {
	if (&_mat != &rhs._mat)
		throw std::runtime_error("iterator not on the same container");
	return _pos == rhs._pos;
}

template <typename T>
bool
iterator<T>::operator!=(const iterator<T>& rhs) const {
	return !(*this == rhs);
}

template <typename T>
T& iterator<T>::operator*() {
	auto res = BinaryToXY(_pos);    // From _pos compute x, y
	return _mat.data[res.y][res.x]; // From x, y retrieve the value
}

template <typename T>
iterator<T>& iterator<T>::operator++() { // prefix
	++_pos;
	return *this;
}

template <typename T>
iterator<T> iterator<T>::operator++(int) { // postfix
	iterator res(_mat, _pos);
	++(*this);
	return res;
}

template <typename T>
iterator<T>& iterator<T>::operator--() { // prefix
	--_pos;
	return *this;
}

template <typename T>
iterator<T> iterator<T>::operator--(int) { // postfix
	iterator res(_mat, _pos);
	--(*this);
	return res;
}

//////////////////
//  CONVERTION  //
//////////////////

template <typename T>
std::size_t
iterator<T>::XYToBinary(Vector<std::size_t> vec) const {
	if (vec.x < 0 || vec.x >= _mat.n || vec.y < 0 || vec.y >= _mat.n)
		throw std::runtime_error("X,Y must be in [0;n-1]");

	std::size_t d = vec.y * _mat.n + vec.x;
	return d ^ (d >> 1);
}

template <typename T>
Vector<std::size_t>
iterator<T>::BinaryToXY(std::size_t d) const {
	if (d < 0 || d > _mat.size() - 1)
		throw std::range_error("Distance must be in range [0, n^2-1]");

	d = d ^ (d >> 16);
	d = d ^ (d >> 8);
	d = d ^ (d >> 4);
	d = d ^ (d >> 2);
	d = d ^ (d >> 1);
	return Vector<std::size_t>(d % _mat.n, d / _mat.n);
}
}
