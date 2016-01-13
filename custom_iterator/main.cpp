#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>

/*! @class MyContainer Custom container (2D Matrix) where iterator follow a
 * Hilbert Curve path.
 * @note cf
 * https://en.wikipedia.org/wiki/Hilbert_curve#Applications_and_mapping_algorithms*/
template <typename T>
class MyContainer {
		std::size_t _n;
		std::vector<std::vector<T>> _data; // row major

	public:
		//! Create a container of n^2 elements aka matrix of n x n elments.
		//! @param[in] n must be a power of two.
		//! @param[in] value value use to initialize the container.
		MyContainer(const std::size_t& n, const T& value=0):
			_n(n)
		, _data(_n, std::vector<T>(_n, value)) {
			if ((n == 0) || (n & (n - 1)))
					throw std::runtime_error("N must be a power of two.");
			std::cout << "MyContainer::MyContainer() called\n";
		}
		~MyContainer() { std::cout << "MyContainer::~MyContainer() called\n"; }

		//convert (x,y) to d
		std::size_t xy2d (std::size_t x, std::size_t y) {
			if (x < 0 ||
					y < 0 ||
					x >= _n ||
					y >= _n)
				throw std::runtime_error("X,Y must be in [0;n-1]");
			std::size_t rx, ry, d=0;
			for (auto s = _n/2; s > 0; s /= 2) {
				rx = (x & s) > 0;
				ry = (y & s) > 0;
				d += s * s * ((3 * rx) ^ ry);
				rot(s, &x, &y, rx, ry);
			}
			return d;
		}

		//convert d to (x,y)
		void d2xy(std::size_t d, std::size_t *x, std::size_t *y) {
			if (d < 0 ||
					d > size()-1)
				throw std::runtime_error("d must be in range [0, n^2-1]");
			std::size_t rx, ry, t=d;
			*x = *y = 0;
			for (std::size_t s=1; s < _n; s *= 2) {
				rx = 1 & (t/2);
				ry = 1 & (t ^ rx);
				rot(s, x, y, rx, ry);
				*x += s * rx;
				*y += s * ry;
				t /= 4;
			}
		}

		//rotate/flip a quadrant appropriately
		void rot(std::size_t n, std::size_t *x, std::size_t *y, std::size_t rx, std::size_t ry) {
			if (ry == 0) {
				if (rx == 1) {
					*x = n-1 - *x;
					*y = n-1 - *y;
				}
				//Swap x and y
				auto t = *x;
				*x = *y;
				*y = t;
			}
		}

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
					// From _pos compute x, y
					std::size_t x, y;
					_container.d2xy(_pos, &x, &y);

					// From x, y retrieve the value
					return _container.data()[y][x];
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

		std::size_t size() const { return std::pow(_n, 2); }
		std::size_t n() const { return _n; }
		std::vector<std::vector<T>>& data() { return _data; }
};

int main() {
	const std::size_t n = (1 << 3);
	MyContainer<int> foo(n);  // 4x4 Matrix
	std::cout << "foo n: " << foo.n() << std::endl;
	std::cout << "foo size: " << foo.size() << std::endl;

	std::iota(foo.begin(), foo.end(), 1);

	std::cout << "foo content:" << std::endl;
	for (auto && i: foo.data()) {
		for (auto && j: i) {
			std::cout << std::setw(4) << j << ' ';
		}
		std::cout << std::endl;
	}

	std::cout << "hilbert curve:" << std::endl;
	for (std::size_t y=0; y < n; ++y) {
		for (std::size_t x=0; x < n; ++x) {
			std::size_t a, b;
			foo.d2xy(foo.xy2d(x, y), &a, &b);
			std::cout << std::setw(4)
								<< "["<< x << ';' << y << "]"
								<< " => "<< foo.xy2d(x, y)
								<< " => ["<< a << ';' << b << "]" << std::endl;
		}
	}
}
