#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <GL/glut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

GLuint window;
GLuint width = 500, height = 500;
std::string name;
std::vector<glm::dvec4> vrhovi;
std::vector<glm::ivec3> trokuti;
std::vector<glm::dvec4 *> ravnine;
glm::dvec4 srediste;
glm::dvec4 tocka;
int brVrhova;
double xMin, xMax, yMin, yMax, zMin, zMax;

void reshape(int width, int height);
void display();
void renderScene();

void racunajRavinu();
bool uSmjeruKazaljke(glm::dvec4 v1, glm::dvec4 v2, glm::dvec4 v3);

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    bool first = true;

    if (file.is_open()) {
        std::string line;

        while(getline(file, line)) {
            std::string front;
            std::stringstream ss(line);

            if (line.front() == 'g') {
                ss >> front >> name;
            } else if (line.front() == 'v') {
                double x, y, z;
                ss >> front >> x >> y >> z;

                if (first) {
                    xMin = xMax = x;
                    yMin = yMax = y;
                    zMin = zMax = z;
                    first = false;
                } else {
                    if (xMin > x) xMin = x;
                    if (xMax < x) xMax = x;
                    if (yMin > y) yMin = y;
                    if (yMax < y) yMax = y;
                    if (zMin > z) zMin = z;
                    if (zMax < z) zMax = z;
                }
                vrhovi.push_back(glm::dvec4(x, y, z, 1.0));
            } else if (line.front() == 'f') {
                int a, b, c;
                ss >> front >> a >> b >> c;

                trokuti.push_back(glm::ivec3(a, b, c));
            }
        }

        file.close();
    }




    /* printf("Trokuti: \n");
    for (int i = 0; i < trokuti.size(); i++) {
        printf("V%d - V%d - V%d\n", trokuti.at(i).x, trokuti.at(i).y, trokuti.at(i).z);
    }
 */
    srediste = glm::dvec4((xMax + xMin)/2, (yMax + yMin)/2,(zMax + zMin)/2, 1.0);
    // printf("SREDISTE V(%f, %f, %f)\n", srediste[0], srediste[1], srediste[2]);


    for (int i = 0; i < vrhovi.size(); i++) {
        vrhovi.at(i).x = 2 * ((vrhovi.at(i).x - xMin) / (xMax - xMin)) - 1;
        vrhovi.at(i).y = 2 * ((vrhovi.at(i).y - yMin) / (yMax - yMin)) - 1;
        vrhovi.at(i).z = 2 * ((vrhovi.at(i).z - zMin) / (zMax - zMin)) - 1;
    }

    std::ifstream fileTocka(argv[2]);
    
    if (fileTocka.is_open()) {
        std::stringstream ss;
        ss << fileTocka.rdbuf();
        fileTocka.close();

        double x, y, z;
        ss >> x >> y >> z;
        // printf("V(%5.2f, %5.2f, %5.2f)\n", x, y, z);
        x = 2 * ((x - xMin) / (xMax - xMin)) - 1;
        y = 2 * ((y - yMin) / (yMax - yMin)) - 1;
        z = 2 * ((z - zMin) / (zMax - zMin)) - 1;
        // printf("V(%5.2f, %5.2f, %5.2f)\n", x, y, z);

        tocka = glm::dvec4(x, y, z, 1);

    }

    /* printf("Vrhovi: \n");
    for (int i = 0; i < vrhovi.size(); i++) {
        printf("V%d(%5.2f, %5.2f, %5.2f)\n", i+1, vrhovi.at(i).x, vrhovi.at(i).y, vrhovi.at(i).z);
    } */

    srediste = glm::dvec4(0.0, 0.0, 0.0, 1.0);

    racunajRavinu();

    printf("Ravnine: \n");
    /* for (int i = 0; i < ravnine.size(); i++) {
        printf("A: %f, B: %f, C: %f, D: %f\n", (*ravnine.at(i))[0], (*ravnine.at(i))[1], (*ravnine.at(i))[2], (*ravnine.at(i))[3]);
    } */

    bool unutarTijela = true;

    for (int i = 0; i < ravnine.size(); i++) {
        glm::dvec4 R = *ravnine[i];
        float dotProduct = R[0] * tocka[0] + R[1] * tocka[1] + R[2] * tocka[2] + R[3] * 1.0;
        // printf("Dot product: %f\n", dotProduct);
        if (dotProduct > 0) {
            unutarTijela = false;
        }
    }

    printf("TOCKA V(%f, %f, %f) JE ", tocka[0], tocka[1], tocka[2]);
    if (unutarTijela) {
        printf("UNUTAR TIJELA!\n");
    } else {
        printf("IZVAN TIJELA!\n");
    }
    
    

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Vjezba 4");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

}


void racunajRavinu() {
    for (int i = 0; i < trokuti.size(); i++) {
        glm::ivec3 trokut = trokuti.at(i);
        int A, B, C, D;
        A = (vrhovi.at(trokut[1] - 1).y - vrhovi.at(trokut[0] - 1).y) * (vrhovi.at(trokut[2] - 1).z - vrhovi.at(trokut[0] - 1).z) - (vrhovi.at(trokut[1] - 1).z - vrhovi.at(trokut[0] - 1).z) * (vrhovi.at(trokut[2] - 1).y - vrhovi.at(trokut[0] - 1).y); // (y2-y1)(z3-z1)-(z2-z1)(y3-y1)
        B = -(vrhovi.at(trokut[1] - 1).x - vrhovi.at(trokut[0] - 1).x) * (vrhovi.at(trokut[2] - 1).z - vrhovi.at(trokut[0] - 1).z) + (vrhovi.at(trokut[1] - 1).z - vrhovi.at(trokut[0] - 1).z) * (vrhovi.at(trokut[2] - 1).x - vrhovi.at(trokut[0] - 1).x);
        C = (vrhovi.at(trokut[1] - 1).x - vrhovi.at(trokut[0] - 1).x) * (vrhovi.at(trokut[2] - 1).y - vrhovi.at(trokut[0] - 1).y) - (vrhovi.at(trokut[1] - 1).y - vrhovi.at(trokut[0] - 1).y) * (vrhovi.at(trokut[2] - 1).x - vrhovi.at(trokut[0] - 1).x);
        D = -vrhovi.at(trokut[0] - 1).x * A - vrhovi.at(trokut[0] - 1).y * B - vrhovi.at(trokut[0] - 1).z * C;
        ravnine.push_back(new glm::dvec4(A, B, C, D));

    }
}

bool uSmjeruKazaljke(glm::dvec4 v1, glm::dvec4 v2, glm::dvec4 v3) {
    std::vector<glm::ivec3> bridovi;

    glm::vec3 brid = glm::cross(glm::vec3(v1), glm::vec3(v2));
    if (glm::dot(brid, glm::vec3(v3)) > 0) {
        return false;
    }

    brid = glm::cross(glm::vec3(v2), glm::vec3(v3));
    if (glm::dot(brid, glm::vec3(v1)) > 0) {
        return false;
    }

    brid = glm::cross(glm::vec3(v3), glm::vec3(v1));
    if (glm::dot(brid, glm::vec3(v2)) > 0) {
        return false;
    }
    return true;
}




void display() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	renderScene();
	glutSwapBuffers();
};

void reshape(int width, int height) {
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width - 1, height - 1, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
};

void renderScene() {
	glPointSize(1.0f);
	glColor3f(0.0f, 0.0f, 0.5f);
    for (int i = 0; i < trokuti.size(); i++) {
        glm::ivec3 trokut = trokuti.at(i);
        glBegin(GL_LINE_LOOP);
        {
            for (int j = 0; j < 3; j++) {
                GLint x = (GLint) int((vrhovi.at(trokut[j] - 1).x + 1.0) * 100) + 100;
                GLint y = (GLint) int((vrhovi.at(trokut[j] - 1).y + 1.0) * 100) + 100;
                // std::cout << "x: " << x << ", y: " << y << std::endl;
                glVertex2i(x, y);
            }
        }
        glEnd();
    }
	
    
    glColor3f(1.0f, 0.5f, 0.0f);
	glPointSize(5.0f);
    glBegin(GL_POINTS);
    {
        GLint x = (GLint) int((tocka.x + 1.0) * 100) + 100;
        GLint y = (GLint) int((tocka.y + 1.0) * 100) + 100;
        // std::cout << "x: " << x << ", y: " << y << std::endl;
        glVertex2i(x, y);
    }
	glEnd();



};
