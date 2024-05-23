#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x3.hpp>
#include <glm/matrix.hpp> 
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BR_TOCKI 3
#define BR_DIM 3


int main(int argc, char** argv) {
    int tockaA[BR_DIM], tockaB[BR_DIM], tockaC[BR_DIM], tockaT[BR_DIM];
    char imena[4] = {'A', 'B', 'C', 'T'};

    for(int i = 0; i < BR_TOCKI + 1; i++) {
        std::cout << "Upisite x, y i z koordinatu za tocku " << imena[i] << ": ";
        switch (i)
        {
        case 0:
            std::cin >> tockaA[0] >> tockaA[1] >> tockaA[2];
            break;
        case 1:
            std::cin >> tockaB[0] >> tockaB[1] >> tockaB[2];
            break;
        case 2:
            std::cin >> tockaC[0] >> tockaC[1] >> tockaC[2];
            break;
        case 3:
            std::cin >> tockaT[0] >> tockaT[1] >> tockaT[2];
            break;
        default:
            break;
        }
    }

    glm::vec3 vecA = glm::make_vec3(tockaA);
    glm::vec3 vecB = glm::make_vec3(tockaB);
    glm::vec3 vecC = glm::make_vec3(tockaC);
    glm::vec3 vecT = glm::make_vec3(tockaT);


    
    glm::mat3x3 mat = glm::transpose(glm::mat3x3(vecA, vecB, vecC));
    std::cout << glm::to_string(mat) << std::endl;
    mat = glm::inverse(mat);

    glm::vec3 vecTBar = mat * vecT;

    std::cout << "Baricentircne koordinate tocke T(" << vecT.x << ", " << vecT.y << ", " << vecT.z << ") su (" << vecTBar.x << ", " << vecTBar.y << ", " << vecTBar.z << ")" << std::endl;

    /* glm::vec3 vec = vecTBar.x * vecA + vecTBar.y * vecB + vecTBar.y * vecC;
    std::cout << glm::to_string(vec) << std::endl; */

}