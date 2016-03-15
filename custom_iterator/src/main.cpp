#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <matrix.hpp>
#include <hilbert_iterator.hpp>
#include <graycode_iterator.hpp>

int
main() {
	const std::size_t n = (1 << 2);
	Matrix<int> foo(n); // 8x8 Matrix
	std::cout << "foo n: " << foo.n << std::endl;
	std::cout << "foo size: " << foo.size() << std::endl;

	{
		std::iota(std::begin(hilbert::iterator<int>(foo)),
		          std::end(hilbert::iterator<int>(foo)),
		          0);
		std::cout << "hilbert curve:" << std::endl << foo << std::endl;

		std::cout << "hilbert Transform:" << std::endl;
		hilbert::iterator<int> it = hilbert::iterator<int>(foo);
		for (std::size_t y = 0; y < n; ++y) {
			for (std::size_t x = 0; x < n; ++x) {
				std::cout << "[" << x << ';' << y << "]";
				std::cout << " => " << it.XYToBinary(Vector<std::size_t>(x, y));
				auto tmp = it.BinaryToXY(it.XYToBinary(Vector<std::size_t>(x, y)));
				std::cout << " => [" << tmp.x << ';' << tmp.y << "]";
				std::cout << std::endl;
			}
		}
	}

	{
		std::iota(std::begin(graycode::iterator<int>(foo)),
		          std::end(graycode::iterator<int>(foo)),
		          0);
		std::cout << "graycode curve:" << std::endl << foo << std::endl;

		std::cout << "graycode Transform:" << std::endl;
		graycode::iterator<int> it = graycode::iterator<int>(foo);
		for (std::size_t y = 0; y < n; ++y) {
			for (std::size_t x = 0; x < n; ++x) {
				std::cout << "[" << x << ';' << y << "]";
				std::cout << " => " << it.XYToBinary(Vector<std::size_t>(x, y));
				auto tmp = it.BinaryToXY(it.XYToBinary(Vector<std::size_t>(x, y)));
				std::cout << " => [" << tmp.x << ';' << tmp.y << "]";
				std::cout << std::endl;
			}
		}
	}
}
