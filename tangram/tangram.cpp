#include <iostream>

#include <tengine/test.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/gtc/type_ptr.hpp>

const float THRESHOLD = (float)1.0e-4;
// random matrix values range: [0, 1.0]
const int OFFSET = 0;
const int RANGE = 10;

struct coordinate_frame {
	glm::vec3 v, w, u;
};

// test

class MathChallenge {

public:
	static struct coordinate_frame calcCoordinateFrame(glm::vec3 view, glm::vec3 up) {
		glm::vec3 v = view / glm::length(view);
		glm::vec3 upxv = glm::cross(up, v);
		glm::vec3 w = upxv / glm::length(upxv);
		glm::vec3 u = glm::cross(v, w);

		return {v, w, u};
	}
	// instead of matrix, you receive the axis of rotation
	static glm::mat3 rodriguesRotation(glm::vec3 axis, float angle) {
		glm::mat3 dual_matrix (
			0       , -axis[2], -axis[1],
			 axis[2], 0		  , -axis[0],
			-axis[1], axis[0] , 0
		);

		float sin = glm::sin(angle);
		float cos = glm::cos(angle);
		glm::mat3 squared = dual_matrix * dual_matrix; 
		return glm::mat3(1.0f) // identity matrix
			 + sin * dual_matrix 
			 + (1 - cos) * squared;
	}

	static void printMatrix(glm::mat3 matrix) {
		glm::mat3 transposed = glm::transpose(matrix);
		for(int i = 0; i < 3; ++i)
				std::cout << glm::to_string(transposed[i]) << std::endl;
	}
	

	static void challenge1(glm::vec3 view, glm::vec3 up, struct coordinate_frame expected) {
		struct coordinate_frame frame = calcCoordinateFrame(view, up);

		std::cout << "view= " << glm::to_string(view) << std::endl;
		std::cout << "up= " << glm::to_string(up) << std::endl;

		std::cout << std::endl;

		std::cout << "v= " << glm::to_string(frame.v) << std::endl;
		std::cout << "w= " << glm::to_string(frame.w) << std::endl;
		std::cout << "u= " << glm::to_string(frame.u) << std::endl;

		if(glm::all(glm::equal(frame.v, expected.v, THRESHOLD)) &
		   glm::all(glm::equal(frame.w, expected.w, THRESHOLD)) &
		   glm::all(glm::equal(frame.u, expected.u, THRESHOLD))) {

			std::cout << "Correct" << std::endl;
		} else {
			std::cout << "Wrong" << std::endl;
		}
		std::cout << "----------------" << std::endl;
	}

	static void challenge2(glm::vec3 axis, glm::vec3 point, double angle, glm::vec3 expected) {
		glm::mat3 rotation = rodriguesRotation(axis, glm::radians(angle));

		std::cout << "axis= " << glm::to_string(axis) << std::endl;
		std::cout << "point= " << glm::to_string(point) << std::endl;
		std::cout << "angle= " << angle << "º" << std::endl;

		std::cout << std::endl;

		glm::vec3 new_point = rotation * point;

		std::cout << "new_point= " << glm::to_string(new_point) << std::endl;

		if(glm::all(glm::equal(new_point, expected, THRESHOLD))) {
			std::cout << "Correct" << std::endl;
		} else {
			std::cout << "Wrong" << std::endl;
		}
		std::cout << "----------------" << std::endl;
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
    std::cout << "Coordinate Frame Challenge" << std::endl;
	std::cout << "----------------" << std::endl;
	MathChallenge::challenge1(glm::vec3(1.0f, 0.0f, 0.0f),
						      glm::vec3(0.0f, 1.0f, 0.0f),
							  {
								glm::vec3(1.000000, 0.000000, 0.000000),
								glm::vec3(0.000000, 0.000000, -1.000000),
								glm::vec3(-0.000000, 1.000000, 0.000000)
							  });
							  
	MathChallenge::challenge1(glm::vec3(1.0f, 0.0f, 0.0f),
						      glm::vec3(0.0f, 0.0f, 1.0f),
							  {
								glm::vec3(1.000000, 0.000000, 0.000000),
								glm::vec3(0.000000, 1.000000, 0.000000),
								glm::vec3(0.000000, 0.000000, 1.000000)
							  });

	MathChallenge::challenge1(glm::vec3(0.0f, 1.0f, 0.0f),
						      glm::vec3(0.0f, 0.0f, 1.0f),
							  {
								glm::vec3(0.000000, 1.000000, 0.000000),
								glm::vec3(-1.000000, 0.000000, 0.000000),
								glm::vec3(0.000000, -0.000000, 1.000000)
							  });

	MathChallenge::challenge1(glm::vec3(0.0f, 1.0f, 0.0f),
						      glm::vec3(1.0f, 0.0f, 0.0f),
							  {
								glm::vec3(0.000000, 1.000000, 0.000000),
								glm::vec3(0.000000, 0.000000, 1.000000),
								glm::vec3(1.000000, 0.000000, 0.000000)
							  });							  
    
	MathChallenge::challenge1(glm::vec3(0.0f, 0.0f, 1.0f),
						      glm::vec3(1.0f, 0.0f, 0.0f),
							  {
								glm::vec3(0.000000, 0.000000, 1.000000),
								glm::vec3(0.000000, -1.000000, 0.000000),
								glm::vec3(1.000000, 0.000000, -0.000000)
							  });

	MathChallenge::challenge1(glm::vec3(0.0f, 0.0f, 1.0f),
						      glm::vec3(0.0f, 1.0f, 0.0f),
							  {
								glm::vec3(0.000000, 0.000000, 1.000000),
								glm::vec3(1.000000, 0.000000, 0.000000),
								glm::vec3(0.000000, 1.000000, 0.000000)
							  });

    MathChallenge::challenge1(glm::vec3(1.0f, 0.0f, 1.0f),
						      glm::vec3(0.0f, 1.0f, 0.0f),
							  {
								glm::vec3(0.707107, 0.000000, 0.707107),
								glm::vec3(0.707107, 0.000000, -0.707107),
								glm::vec3(-0.000000, 1.000000, 0.000000)
							  });								

	MathChallenge::challenge1(glm::vec3(0.0f, 1.0f, 1.0f),
						      glm::vec3(1.0f, 0.0f, 0.0f),
							  {
								glm::vec3(0.000000, 0.707107, 0.707107),
								glm::vec3(0.000000, -0.707107, 0.707107),
								glm::vec3(1.000000, 0.000000, -0.000000)
							  });	

	MathChallenge::challenge1(glm::vec3(1.0f, 1.0f, 0.0f),
						      glm::vec3(0.0f, 0.0f, 1.0f),
							  {
								glm::vec3(0.707107, 0.707107, 0.000000),
								glm::vec3(-0.707107, 0.707107, 0.000000),
								glm::vec3(0.000000, -0.000000, 1.000000)
							  });

	MathChallenge::challenge1(glm::vec3(1.0f, 1.0f, 0.0f),
						      glm::vec3(0.0f, 1.0f, 0.0f),
							  {
								glm::vec3(0.707107, 0.707107, 0.000000),
								glm::vec3(0.000000, 0.000000, -1.000000),
								glm::vec3(-0.707107, 0.707107, 0.000000)
							  });	

    std::cout << std::endl << "Rodrigues Rotation Challenge" << std::endl;
	std::cout << "----------------" << std::endl;						

	MathChallenge::challenge2(glm::vec3(1.0f, 0.0f, 0.0f), // axis
							  glm::vec3(0.0f, 1.0f, 0.0f), // point
							  180.0f,         			   // angel
							  glm::vec3(0.0f, -1.0f, 0.0f));

	MathChallenge::challenge2(glm::vec3(1.0f, 0.0f, 0.0f), 
							  glm::vec3(0.0f, 1.0f, 0.0f),
							  90.0f,
							  glm::vec3(0.0f, 0.0f, -1.0f));

	MathChallenge::challenge2(glm::vec3(0.0f, 1.0f, 0.0f), 
							  glm::vec3(1.0f, 0.0f, 0.0f),
							  180.0f,
							  glm::vec3(3.0f, 0.0f, 0.0f));

	MathChallenge::challenge2(glm::vec3(0.0f, 1.0f, 0.0f), 
							  glm::vec3(1.0f, 0.0f, 0.0f),
							  90.0f,
							  glm::vec3(2.0f, 0.0f, -1.0f));

	MathChallenge::challenge2(glm::vec3(1.0f, 0.0f, 1.0f), 
							  glm::vec3(1.0f, 1.0f, 1.0f),
							  180.0f,
							  glm::vec3(1.0f, -3.0f, 1.0f));		

	MathChallenge::challenge2(glm::vec3(1.0f, 0.0f, 1.0f), 
							  glm::vec3(1.0f, 1.0f, 1.0f),
							  90.0f,
							  glm::vec3(2.0f, -1.0f, 0.0f));

	MathChallenge::challenge2(glm::vec3(1.0f, 1.0f, 0.0f), 
							  glm::vec3(0.0f, 1.0f, 1.0f),
							  270.0f,
							  glm::vec3(2.0f, -1.0f, 2.0f));

	MathChallenge::challenge2(glm::vec3(1.0f, 1.0f, 0.0f), 
							  glm::vec3(0.0f, 1.0f, 1.0f),
							  45.0f,
							  glm::vec3(-0.414214f, 1.414214f, 0.292893f));

	MathChallenge::challenge2(glm::vec3(1.0f, 0.0f, 0.0f), 
							  glm::vec3(1.0f, 0.0f, 0.0f),
							  300.0f,
							  glm::vec3(1.0f, 0.0f, 0.0f));	

	MathChallenge::challenge2(glm::vec3(1.0f, 0.0f, 0.0f), 
							  glm::vec3(3.14f, 1.0f, -8.0f),
							  180.0f,
							  glm::vec3(3.14f, -1.0f, 8.0f));

	// Providing a seed value
	
	srand((unsigned)time(NULL));

	for (int i = 0; i < 100; i++)
		MathChallenge::challenge3();

	return 0;
}