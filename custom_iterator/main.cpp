#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>

/*! @class MyContainer Custom container (2D Matrix) where iterator follow a
 * Hilbert Curve path.
 * @note cf
 * https://en.wikipedia.org/wiki/Hilbert_curve#Applications_and_mapping_algorithms*/
template <typename T>
class MyContainer {
 public:
	//! Create a container of n^2 elements aka matrix of n x n elments.
	//! @param[in] n must be a power of two.
	//! @param[in] value value use to initialize the container.
	MyContainer(const std::size_t& n, const T& value = 0)
			: n(n), _data(n, std::vector<T>(n, value)) {
		if ((n == 0) || (n & (n - 1)))
			throw std::runtime_error("N must be a power of two.");
		std::cout << "MyContainer::MyContainer() called\n";
	}
	~MyContainer() { std::cout << "MyContainer::~MyContainer() called\n"; }

	class iterator {
	 public:
		iterator(MyContainer& container, std::size_t pos = 0)
				: _container(container), _pos(pos) {}

		bool operator==(const iterator& rhs) {
			if (&_container != &rhs._container)
				throw std::runtime_error("iterator not on the same container");
			return _pos == rhs._pos;
		}

		bool operator!=(const iterator& rhs) { return !(*this == rhs); }

		T& operator*() {
			return _container.data()[_pos / _container.n][_pos % _container.n];
		}

		iterator& operator++() {  // prefix
			++_pos;
			return *this;
		}
		iterator operator++(int) {  // postfix
			iterator res(_container, _pos);
			++(*this);
			return res;
		}

		iterator& operator--() {  // prefix
			--_pos;
			return *this;
		}
		iterator operator--(int) {  // postfix
			iterator res(_container, _pos);
			--(*this);
			return res;
		}

	 private:
		MyContainer& _container;
		std::size_t _pos;
	};
	iterator begin() { return iterator(*this); }
	iterator end() { return iterator(*this, size()); }

	std::size_t n;
	std::size_t size() const { return std::pow(n, 2); }
	std::vector<std::vector<T>>& data() { return _data; }

/*{
		std::size_t count = 0;
		for (auto const& it : container) {
			stream << std::setw(4) << it << ' ';
			if (++count == container.n) {
				count = 0;
				stream << std::endl;
			}
		}
		return stream;
	}*/

 private:
	std::vector<std::vector<T>> _data;  // row major
};

template <typename T>
class iteratorHilbert {
 public:
	iteratorHilbert(MyContainer<T>& container, std::size_t pos = 0);

	iteratorHilbert<T> begin();
	iteratorHilbert<T> end();

	bool operator==(const iteratorHilbert& rhs);
	bool operator!=(const iteratorHilbert& rhs);
	T& operator*();
	iteratorHilbert& operator++();    // prefix
	iteratorHilbert& operator--();    // prefix
	iteratorHilbert operator++(int);  // postfix
	iteratorHilbert operator--(int);  // postfix

 private:
	MyContainer<T>& _container;
	std::size_t _pos;

	// convert (x,y) to d
	std::size_t xy2d(std::size_t x, std::size_t y) {
		if (x < 0 || x >= _container.n || y < 0 || y >= _container.n)
			throw std::runtime_error("X,Y must be in [0;n-1]");
		std::size_t rx, ry, d = 0;
		for (auto s = _container.n / 2; s > 0; s /= 2) {
			rx = (x & s) > 0;
			ry = (y & s) > 0;
			d += s * s * ((3 * rx) ^ ry);
			rot(s, &x, &y, rx, ry);
		}
		return d;
	}

	// convert d to (x,y)
	void d2xy(std::size_t d, std::size_t* x, std::size_t* y) {
		if (d < 0 || d > _container.size() - 1)
			throw std::runtime_error("d must be in range [0, n^2-1]");
		std::size_t rx, ry, t = d;
		*x = *y = 0;
		for (std::size_t s = 1; s < _container.n; s *= 2) {
			rx = 1 & (t / 2);
			ry = 1 & (t ^ rx);
			rot(s, x, y, rx, ry);
			*x += s* rx;
			*y += s* ry;
			t /= 4;
		}
	}

	// rotate/flip a quadrant appropriately
	void rot(std::size_t n,
					 std::size_t* x,
					 std::size_t* y,
					 std::size_t rx,
					 std::size_t ry) {
		if (ry == 0) {
			if (rx == 1) {
				*x = n - 1 - *x;
				*y = n - 1 - *y;
			}
			// Swap x and y
			auto t = *x;
			*x = *y;
			*y = t;
		}
	}
};

template<typename T, template <typename> class U>
iteratorHilbert<T> make_HilbertIterator(U<T>& p) {
		return iteratorHilbert<T>(p);
}

template <typename T>
std::ostream& operator<<(std::ostream &stream, MyContainer<T>& container);

int main() {
	const std::size_t n = (1 << 3);
	using ContainerInt = MyContainer<int>;
	using HilberIntIterator = iteratorHilbert<int>;

	MyContainer<int> foo(n);  // 4x4 Matrix
	std::cout << "foo n: " << foo.n << std::endl;
	std::cout << "foo size: " << foo.size() << std::endl;

	std::iota(make_HilbertIterator(foo).begin(), make_HilbertIterator(foo).end(), 1);

	std::cout << "foo content:" << std::endl << foo << std::endl;
	/*
	std::size_t count = 0;
	for (auto const& it : foo) {
		std::cout << std::setw(4) << it << ' ';
		if (++count == n) {
			count = 0;
			std::cout << std::endl;
		}
	}
	*/

	/*std::cout << "hilbert curve:" << std::endl;
	for (std::size_t y=0; y < n; ++y) {
		for (std::size_t x=0; x < n; ++x) {
			std::size_t a, b;
			foo.d2xy(foo.xy2d(x, y), &a, &b);
			std::cout << std::setw(4)
								<< "["<< x << ';' << y << "]"
								<< " => "<< foo.xy2d(x, y)
								<< " => ["<< a << ';' << b << "]"
								<< std::endl;
		}
	}*/
}

///////////////////
//  MyContainer  //
///////////////////

template <typename T>
std::ostream& operator<<(std::ostream &stream, MyContainer<T>& container) {
	std::size_t count = 0;
	for (auto const& it : container) {
		stream << std::setw(4) << it << ' ';
		if (++count == container.n) {
			count = 0;
			stream << std::endl;
		}
	}
	return stream;
}

///////////////////////
//  iteratorHilbert  //
///////////////////////
template <typename T>
iteratorHilbert<T>::iteratorHilbert(MyContainer<T>& container, std::size_t pos)
		: _container(container), _pos(pos) {
}

template <typename T>
iteratorHilbert<T> iteratorHilbert<T>::begin() {
	return iteratorHilbert<T>(_container);
}

template <typename T>
iteratorHilbert<T> iteratorHilbert<T>::end() {
	return iteratorHilbert<T>(_container, _container.size());
}

template <typename T>
bool iteratorHilbert<T>::operator==(const iteratorHilbert& rhs) {
	if (&_container != &rhs._container)
		throw std::runtime_error("iterator not on the same container");
	return _pos == rhs._pos;
}

template <typename T>
bool iteratorHilbert<T>::operator!=(const iteratorHilbert& rhs) {
	return !(*this == rhs);
}

template <typename T>
T& iteratorHilbert<T>::operator*() {
	std::size_t x, y;
	d2xy(_pos, &x, &y);              // From _pos compute x, y
	return _container.data()[y][x];  // From x, y retrieve the value
}

template <typename T>
iteratorHilbert<T>& iteratorHilbert<T>::operator++() {  // prefix
	++_pos;
	return *this;
}

template <typename T>
iteratorHilbert<T> iteratorHilbert<T>::operator++(int) {  // postfix
	iteratorHilbert res(_container, _pos);
	++(*this);
	return res;
}

template <typename T>
iteratorHilbert<T>& iteratorHilbert<T>::operator--() {  // prefix
	--_pos;
	return *this;
}

template <typename T>
iteratorHilbert<T> iteratorHilbert<T>::operator--(int) {  // postfix
	iteratorHilbert res(_container, _pos);
	--(*this);
	return res;
}
