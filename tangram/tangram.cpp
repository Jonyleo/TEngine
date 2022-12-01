/*
	Assignment 1: GROUP 1
		93728	| João Miguel Ferreira Oliveira
		105147	| Giulio Camasso
*/
#include <iostream>

#include <tengine/test.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtc/type_ptr.hpp>

#define THRESHOLD (float)1.0e-5
#define MIN_RAND 10
#define MAX_RAND 11

class MathChallenge {

public:
	static glm::mat3 calcCoordinateFrame(glm::vec3 view, glm::vec3 up) {
		glm::vec3 v = view / glm::length(view);
		glm::vec3 upxv = glm::cross(up, v);
		glm::vec3 w = upxv / glm::length(upxv);
		glm::vec3 u = glm::cross(v, w);

		return {v, w, u};
	}
	
	static glm::mat3 rodriguesRotation(glm::vec3 axis, float angle) {
		glm::mat3 dual_matrix (
			0       , -axis[2], -axis[1],
			 axis[2], 0		  , -axis[0],
			-axis[1], axis[0] , 0
		);

		float sin = glm::sin(angle);
		float cos = glm::cos(angle);
		glm::mat3 squared = dual_matrix * dual_matrix; 
		
		return glm::mat3(1.0f)
			+ sin * dual_matrix 
			+ (1 - cos) * squared;
	}

	static void printMat3(glm::mat3 matrix) {
		glm::mat3 transposed = glm::transpose(matrix);
		for(int i = 0; i < 3; ++i)
			std::cout << glm::to_string(transposed[i]) << std::endl;
	}

	static float randNumber(int min, int max) {
		return (rand() % min) + (max - min - 1); 
	}

	static glm::vec3 randVec3(int min, int max) {
		return glm::vec3(randNumber(min,max),
						 randNumber(min,max),
						 randNumber(min,max));
	}

	static glm::mat3 randMat3(int min, int max) {
		return glm::mat3(randVec3(min,max),
						 randVec3(min,max),
						 randVec3(min,max));
	}
	
	static void challenge1(glm::vec3 view, glm::vec3 up, glm::mat3 expected) {
		glm::mat3 frame = calcCoordinateFrame(view, up);

		std::cout << "view= " << glm::to_string(view) << std::endl;
		std::cout << "up= " << glm::to_string(up) << std::endl;

		std::cout << std::endl;

		std::cout << "v= " << glm::to_string(frame[0]) << std::endl;
		std::cout << "w= " << glm::to_string(frame[1]) << std::endl;
		std::cout << "u= " << glm::to_string(frame[2]) << std::endl;

		if(glm::all(glm::equal(frame, expected, THRESHOLD)))
			std::cout << "Correct" << std::endl;
		else
			std::cout << "Wrong" << std::endl;
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

		if(glm::all(glm::equal(new_point, expected, THRESHOLD)))
			std::cout << "Correct" << std::endl;
		else
			std::cout << "Wrong" << std::endl;

		std::cout << "----------------" << std::endl;
	}

	static void testEquation(glm::mat3 A, glm::mat3 B){
		
		glm::mat3 firstMember = glm::inverse(A * B);
		std::cout << "\n(AB)^-1:" << std::endl;
		printMat3(firstMember);

		glm::mat3 secondMember = glm::inverse(B) * glm::inverse(A);
		std::cout << "\n(B^-1) (A^-1):" << std::endl;
		printMat3(secondMember);

		if (glm::all(glm::equal(firstMember, secondMember, THRESHOLD)))
			std::cout << "Correct" << std::endl;
		else
			std::cout << "Wrong" << std::endl;

		std::cout << "----------------" << std::endl;
		
	}
	
	static void challenge3() {

		// random matrix creation: A
		glm::mat3 A = randMat3(10, 11);
		// std::cout << std::endl << "A:" << std::endl;
		// printMat3(A);
		float detA = glm::determinant(A);
		// std::cout << std::endl << "detA: " << detA << std::endl;

		if(detA == 0) {
			std::cout << std::endl << "detA = 0. Return...\n";
			return;
		}

		// random matrix creation: B
		glm::mat3 B = randMat3(10, 11);
		// std::cout << std::endl << "B:" << std::endl;
		// printMat3(B);
		float detB = glm::determinant(B);
		// std::cout << std::endl << "detB: " << detB << std::endl;

		if (detB == 0) {
			std::cout << std::endl << "detB = 0. Return...\n";
			return;
		}

		// prove: (AB)^-1 = (B^-1) (A^-1)
		testEquation(A, B);
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


    std::cout << std::endl << "Inversion Challenge" << std::endl;
	std::cout << "----------------" << std::endl;		

	// Providing a seed value
	srand((unsigned)time(NULL));

	for (int i = 0; i < 10; i++)
		MathChallenge::challenge3();

	return 0;
}