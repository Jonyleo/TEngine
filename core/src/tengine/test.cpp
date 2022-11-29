#include <iostream>
#include <glm/glm.hpp>

#include "tengine/test.hpp"

tengine::Test::Test() {}

void tengine::Test::print() {
	std::cout << "Hello core!" << std::endl;
	glm::vec3 position(0.0f);

	std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
}