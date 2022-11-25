#include <iostream>

#include "tengine/core/test.hpp"

tengine::core::Test::Test() {}

void tengine::core::Test::print() {
	std::cout << "Hello core!" << std::endl;
}