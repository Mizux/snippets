#pragma once

#include <matrix.hpp>

#include <stdexcept>

template <typename T>
Matrix<T>::Matrix(std::size_t n, const T& value)
  : n(n)
  , data(n, std::vector<T>(n, value)) {
	if ((n == 0) || (n & (n - 1))) throw std::runtime_error("N must be a power of two.");
	// std::cout << "Matrix::Matrix() called\n";
}

template <typename T>
Matrix<T>::~Matrix() {
	// std::cout << "Matrix::~Matrix() called\n";
}

template <typename T>
std::size_t
Matrix<T>::size() const {
	return std::pow(n, 2);
}

template <typename T>
typename Matrix<T>::iterator
Matrix<T>::begin() {
	return iterator(*this);
}

template <typename T>
typename Matrix<T>::iterator
Matrix<T>::end() {
	return iterator(*this, size());
}

template <typename T>
typename Matrix<T>::const_iterator
Matrix<T>::begin() const {
	return const_iterator(*this);
}

template <typename T>
typename Matrix<T>::const_iterator
Matrix<T>::end() const {
	return const_iterator(*this, size());
}

////////////////
//  iterator  //
////////////////

template <typename T>
Matrix<T>::iterator::iterator(Matrix<T>& mat, std::size_t pos)
  : _mat(mat)
  , _pos(pos) {}

template <typename T>
bool
Matrix<T>::iterator::operator==(const iterator& rhs) {
	if (&_mat != &rhs._mat)
		throw std::runtime_error("iterator not on the same container");
	return _pos == rhs._pos;
}

template <typename T>
bool
Matrix<T>::iterator::operator!=(const iterator& rhs) {
	return !(*this == rhs);
}

template <typename T>
T& Matrix<T>::iterator::operator*() {
	return _mat.data[_pos / _mat.n][_pos % _mat.n];
}

template <typename T>
typename Matrix<T>::iterator& Matrix<T>::iterator::operator++() { // prefix
	++_pos;
	return *this;
}

template <typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::operator++(int) { // postfix
	iterator res(_mat, _pos);
	++(*this);
	return res;
}

template <typename T>
typename Matrix<T>::iterator& Matrix<T>::iterator::operator--() { // prefix
	--_pos;
	return *this;
}

template <typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::operator--(int) { // postfix
	iterator res(_mat, _pos);
	--(*this);
	return res;
}

//////////////////////
//  const_iterator  //
//////////////////////

template <typename T>
Matrix<T>::const_iterator::const_iterator(const Matrix<T>& mat, std::size_t pos)
  : _mat(mat)
  , _pos(pos) {}

template <typename T>
bool
Matrix<T>::const_iterator::operator==(const const_iterator& rhs) {
	if (&_mat != &rhs._mat)
		throw std::runtime_error("const_iterator not on the same container");
	return _pos == rhs._pos;
}

template <typename T>
bool
Matrix<T>::const_iterator::operator!=(const const_iterator& rhs) {
	return !(*this == rhs);
}

template <typename T>
const T& Matrix<T>::const_iterator::operator*() {
	return _mat.data[_pos / _mat.n][_pos % _mat.n];
}

template <typename T>
typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::operator++() { // prefix
	++_pos;
	return *this;
}

template <typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::operator++(
  int) { // postfix
	const_iterator res(_mat, _pos);
	++(*this);
	return res;
}

template <typename T>
typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::operator--() { // prefix
	--_pos;
	return *this;
}

template <typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::operator--(
  int) { // postfix
	const_iterator res(_mat, _pos);
	--(*this);
	return res;
}

////////////
//  Misc  //
////////////

template <typename T>
std::ostream&
operator<<(std::ostream& stream, const Matrix<T>& mat) {
	std::size_t count = 0;
	for (auto const& it : mat) {
		stream << std::setw(4) << it << ' ';
		if (++count == mat.n) {
			count = 0;
			stream << std::endl;
		}
	}
	return stream;
}
