#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x3.hpp>
#include <glm/matrix.hpp> 
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BR_JED 3
#define BR_VAR 3


int main(int argc, char** argv) {
    int arr[BR_JED * BR_VAR];
    int t[BR_JED];
    std::cout << "Racunanje sustava tri jednadzbe s tri nepoznanice" << std::endl;

    for(int i = 0; i < BR_JED; i++) {
        std::cout << "Upisite a, b, c i d vrijednosti za " << i + 1 << ". jednadzbu oblika a*x + b*y + c*z = d: ";
        std::cin >> arr[i*BR_VAR + 0] >> arr[i*BR_VAR + 1] >> arr[i*BR_VAR + 2] >> t[i];
    }

    /* for (int i = 0; i < BR_JED * BR_VAR; i++) {
        std::cout << arr[i] << " ";
        if (i % 3 == 2) std::cout << t[i / 3] << std::endl;
    } */

    glm::mat3x3 mat = glm::make_mat3x3(arr);
    mat = glm::inverse(glm::transpose(mat));

    if(glm::isnan(glm::determinant(mat))) {
        std::cout << "Ne postoji rjesenje za zadane jednadzbe!";
        return 0;
    }

    glm::vec3 vec = glm::make_vec3(t);

    glm::vec3 solution = mat * vec;

    std::cout << "[x y z] = [" << solution.x << " " << solution.y << " " << solution.z << "]";



}