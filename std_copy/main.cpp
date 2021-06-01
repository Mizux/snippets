#include <vector>
#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& out, const std::vector<int>& input) {
    for(const auto i: input) {
        out << i << ", ";
    }
    return out;
}

int main()
{
    std::vector<std::vector<int>> v{{1,2,3},{4,5},{6}, {7}, {}, {8, 9, 10}};
    std::cout << "v: ";
    for(const auto& inner: v)
        std::cout << "{" << inner << "} ";
    std::cout << std::endl;

    std::vector<std::vector<int>> d;
    std::copy_if(
        v.begin(),
        v.end(),
        std::back_inserter(d),
        [](const std::vector<int>& it) -> bool { return it.size() > 1;});
    std::cout << "d: ";
    for(const auto& inner: d)
        std::cout << "{" << inner << "} ";
    std::cout << std::endl;

    // Flatten v as a 1D vector
    std::vector<int> n{};//new 1D vector
    for(const auto& vec: v) {
        //std::copy(vec.begin(), vec.end(), std::back_inserter(n));
        n.insert(n.end(), vec.begin(), vec.end());
    }
    std::cout << "n: {" << n << "}" << std::endl;
}
