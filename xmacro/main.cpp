#include <iostream>
#include <map>

#define FOOBAR_LIST \
 X(FOO), \
 X(BAR), \
 X(INVALID)

enum class FooBar {
#define X(name) name //!< @brief XMacro stuff.
	FOOBAR_LIST
#undef X
};

std::ostream& operator<<(std::ostream& stream, const FooBar& p);

int
main() {
	FooBar foo = FooBar::FOO;
	std::cout << "foo: " << foo << std::endl;
	FooBar bar = FooBar::BAR;
	std::cout << "bar: " << bar << std::endl;
	FooBar invalid = FooBar::INVALID;
	std::cout << "unknow: " << invalid << std::endl;

	switch (bar) {
		case FooBar::FOO:
		case FooBar::INVALID:
		default:
			std::cout << "Here" << std::endl;
			break;
	} // warning since BAR is not managed with -Wswitch-enum
}

std::ostream&
operator<<(std::ostream& stream, const FooBar& value) {
	static const std::map<FooBar, std::string> fooBarNames = {
#define X(name) { FooBar::name, #name } //!< @brief XMacro stuff.
	  FOOBAR_LIST
#undef X
	};

	stream << fooBarNames.find(value)->second;
	return stream;
}
