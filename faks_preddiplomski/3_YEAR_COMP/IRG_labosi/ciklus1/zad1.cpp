#include <iostream>
#include "string"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp> 
#include <glm/gtx/string_cast.hpp>

void printVector3(std::string name, glm::vec3 vector) {
	std::cout << name << " = " << vector.x << "i ";
	if(vector.y >= 0) std::cout << "+";
	std::cout << vector.y << "j ";
	if(vector.z >= 0) std::cout << "+";
	std::cout << vector.z << "k" << std::endl;
}

void printMatrix3x3(std::string name, glm::mat3x3 mat) {
	for(int i = 0; i < name.size() + 4; i++) {
		std::cout << " ";
	}
	for(int i = 0; i < 23; i++) {
		std::cout << "_";
	}
	std::cout << std::endl;
	std::cout << name << " = |";
	std::printf("%7.2f %7.2f %7.2f", mat[0][0], mat[0][1], mat[0][2]);
	std::cout << "|" << std::endl;
	for(int i = 0; i < name.size() + 3; i++) {
		std::cout << " ";
	}
	std::cout << "|";
	std::printf("%7.2f %7.2f %7.2f", mat[1][0], mat[1][1], mat[1][2]);
	std::cout << "|" << std::endl;
	for(int i = 0; i < name.size() + 3; i++) {
		std::cout << " ";
	}
	std::cout << "|";
	std::printf("%7.2f %7.2f %7.2f", mat[2][0], mat[2][1], mat[2][2]);
	std::cout << "|" << std::endl;
	
}

int main(int argc, char** argv) {
	
	glm::vec3 v1 = glm::vec3(2, 3, -4) + glm::vec3(-1, 4, -1);
	printVector3("v1", v1);
	float s = glm::dot( v1, glm::vec3(-1, 4, -1));
	std::cout << s;
	glm::vec3 v2 = glm::cross(v1, glm::vec3(2, 2, 4));
	printVector3("v2", v2);
	glm::vec3 v3 = glm::normalize(v2);
	printVector3("v3", v3);
	glm::vec3 v4 = -v2;
	printVector3("v4", v4);

	glm::mat3x3 m1 = glm::mat3x3(glm::vec3(1, 2, 3), glm::vec3(2, 1, 3), glm::vec3(4, 5, 1)) + glm::mat3x3(glm::vec3(-1, 2, -3), glm::vec3(5, -2, 7), glm::vec3(-4, -1, 3));

	printMatrix3x3("M1", m1);

	glm::mat3x3 m2 = glm::mat3x3(glm::vec3(1, 2, 3), glm::vec3(2, 1, 3), glm::vec3(4, 5, 1)) * glm::transpose(glm::mat3x3(glm::vec3(-1, 2, -3), glm::vec3(5, -2, 7), glm::vec3(-4, -1, 3)));

	printMatrix3x3("M2", m2);

	glm::mat3x3 m3 = glm::mat3x3(glm::vec3(1, 2, 3), glm::vec3(2, 1, 3), glm::vec3(4, 5, 1)) * glm::inverse(glm::mat3x3(glm::vec3(-1, 2, -3), glm::vec3(5, -2, 7), glm::vec3(-4, -1, 3)));

	printMatrix3x3("M3", m3);

	glm::vec4 V = glm::vec4(1, 2, 3, 4) * glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 2, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(2, 3, 3, 1));

	std::cout << "V = " << glm::to_string((glm::ivec4)V) << std::endl;

}
