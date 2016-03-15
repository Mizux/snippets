#include <vector>
#include <algorithm>

template <class InputIterator, class ValueIterator>
std::vector<InputIterator>
find_tuple(InputIterator first,
           InputIterator last,
           ValueIterator valFirst,
           ValueIterator valLast) {
	std::vector<InputIterator> inputs(std::distance(first, last));
	std::iota(inputs.begin(), inputs.end(), first);

	std::vector<InputIterator> result;
	result.reserve(std::distance(valFirst, valLast));

	// Find first element equal to v, remove it from inputs and add it to res.
	// need "const auto&" (c++14) to work with std::array (return T* not an
	// iterator on begin()/end() )
	auto func = [&result, &inputs](const auto& v) {
		for (auto it = inputs.begin(); it != inputs.end(); ++it) {
			if (**it == v) {
				result.push_back(*it);
				inputs.erase(it);
				break;
			}
		}
	};
	std::for_each(valFirst, valLast, func);
	if (result.size() == std::distance(valFirst, valLast))
		return result;
	else
		return std::vector<InputIterator>();
}

#include <list>
#include <set>
#include <array>
#include <algorithm>
#include <iostream>

int
main() {
	std::list<int> l(10);
	std::iota(l.begin(), l.end(), 0);

	std::cout << "contents of the list: ";
	for (auto n : l)
		std::cout << n << ' ';
	std::cout << '\n';

	{
		std::array<int, 3> vals;
		std::iota(vals.begin(), vals.end(), 4);

		std::cout << "contents of the value (array): ";
		for (auto n : vals)
			std::cout << n << ' ';
		std::cout << '\n';

		auto res = find_tuple(l.begin(), l.end(), vals.begin(), vals.end());
		std::cout << "contents of the result: ";
		for (auto it : res)
			std::cout << *it << ' ';
		std::cout << '\n';
	}

	{
		std::vector<int> vals = {4, 5, 6};

		std::cout << "contents of the value (vector): ";
		for (auto n : vals)
			std::cout << n << ' ';
		std::cout << '\n';

		auto res = find_tuple(l.begin(), l.end(), vals.begin(), vals.end());
		std::cout << "contents of the result: ";
		for (auto it : res)
			std::cout << *it << ' ';
		std::cout << '\n';
	}

	{
		std::set<int> vals = {4, 5, 6};

		std::cout << "contents of the value (set): ";
		for (auto n : vals)
			std::cout << n << ' ';
		std::cout << '\n';

		auto res = find_tuple(l.begin(), l.end(), vals.begin(), vals.end());
		std::cout << "contents of the result: ";
		for (auto it : res)
			std::cout << *it << ' ';
		std::cout << '\n';
	}
}
