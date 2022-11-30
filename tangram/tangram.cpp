#include <iostream>
#include <tengine/test.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

const float THRESHOLD = (float)1.0e-4;
// random matrix values range: [0, 1.0]
const int OFFSET = 0;
const int RANGE = 10;

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
	

	static void challenge3(){
		
		// random matrices creation: A
		float v[9];

		for (int i = 0; i < 9; ++i)
				v[i] = 0.1f * OFFSET + 0.1f * (rand() % RANGE);

		glm::mat3 A = glm::make_mat3(v);
		std::cout << "\nA:\n";
		printMatrix(A);
		float detA = glm::determinant(A);
		std::cout << "\ndetA: " << detA << std::endl;

		if(detA == 0)
			return;

		// random matrices creation: B
		for (int i = 0; i < 9; ++i)
				v[i] = 0.1f * OFFSET + 0.1f * (rand() % RANGE);

		glm::mat3 B = glm::make_mat3(v);

		std::cout << "\nB:\n";
		printMatrix(B);
		float detB = glm::determinant(B);
		std::cout << "\ndetB: " << detB << std::endl;
		if (detB == 0)
			return;

		// (AB)^-1 = (B^-1) (A^-1)
		glm::mat3 firstMember = glm::inverse(A * B);
		std::cout << "\n(AB)^-1:\n";
		printMatrix(firstMember);

		glm::mat3 secondMember = glm::inverse(B) * glm::inverse(A);
		std::cout << "\n(B^-1) (A^-1):\n";
		printMatrix(secondMember);
		
		// transpose operation not required here
		for (int i = 0; i < 3; ++i)
			assert(glm::all(glm::equal(firstMember[i], secondMember[i], THRESHOLD)));
	}
};


int main() {
	/*
	glm::vec3 view(1.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	struct coordinate_frame frame = MathChallenge::calcCoordinateFrame(view, up);

	std::cout << "v= " << glm::to_string(frame.v) << std::endl;
	std::cout << "w= " << glm::to_string(frame.w) << std::endl;
	std::cout << "u= " << glm::to_string(frame.u) << std::endl;
	*/
	// Providing a seed value
	
	srand((unsigned)time(NULL));

	for (int i = 0; i < 100; i++)
		MathChallenge::challenge3();

	return 0;
}