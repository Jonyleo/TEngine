#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

struct coordinate_frame {
	glm::vec3 v, w, u;
};

class MathChallenge {
public:
	static struct coordinate_frame calcCoordinateFrame(glm::vec3 view, glm::vec3 up) {
		glm::vec3 v = view / glm::length(view);
		glm::vec3 upxv = glm::cross(up, v);
		glm::vec3 w = upxv / glm::length(upxv);
		glm::vec3 u = glm::cross(v, w);

		return {v, w, u};
	}

	static glm::mat3 rodriguesRotation(glm::mat3 matrix, float angle) {
		float sin = glm::sin(angle);
		float cos = glm::cos(angle);
		glm::mat3 squared = matrix * matrix;
		return glm::mat3(1.0f) // identity matrix
			 + sin * matrix 
			 + (1 - cos) * squared;
	}

	static void printMatrix(glm::mat3 matrix) {
		glm::mat3 transposed = glm::transpose(matrix);
		for(int i = 0; i < 3; ++i)
			std::cout << glm::to_string(transposed[i]) << std::endl;
	}

};


int main() {
	glm::vec3 view(1.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	struct coordinate_frame frame = MathChallenge::calcCoordinateFrame(view, up);

	std::cout << "v= " << glm::to_string(frame.v) << std::endl;
	std::cout << "w= " << glm::to_string(frame.w) << std::endl;
	std::cout << "u= " << glm::to_string(frame.u) << std::endl;

	return 0;
}