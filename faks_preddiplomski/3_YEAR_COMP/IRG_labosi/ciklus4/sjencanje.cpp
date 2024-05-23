#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <GL/glut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

GLuint window;
GLuint width = 500, height = 500;
std::string name;
glm::vec4 ociste;
glm::vec4 glediste;
glm::vec4 izvor;
glm::dvec4 L;
glm::mat4 T, P;
std::vector<glm::dvec4> vrhovi;
std::vector<glm::ivec3> trokuti;
std::vector<glm::dvec3> normale;
std::map<int, std::vector<int>> poligoniVrha;
glm::dvec4 srediste;
int brVrhova;
double xMin, xMax, yMin, yMax, zMin, zMax;
int Ia, Ii;
double ka, kd;
char koordinata = 'q';

void reshape(int width, int height);
void display();
void renderScene();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);

void racunajTiP();

int main(int argc, char** argv) {
    std::ifstream file("koordinate.txt");
    

    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        double x, y, z;

        ss >> x >> y >> z;
        ociste = glm::dvec4(x, y, z, 1);

        ss >> x >> y >> z;
        glediste = glm::dvec4(x, y, z, 1);
        
    }

    std::ifstream fileIzvor("izvor.txt");
    

    if (fileIzvor.is_open()) {
        std::stringstream ss;
        ss << fileIzvor.rdbuf();
        fileIzvor.close();

        double x, y, z;

        ss >> x >> y >> z;
        izvor = glm::dvec4(x, y, z, 1);
        double lenIzvor = sqrt(x * x + y* y + z * z);
        L = glm::dvec4(izvor.x / lenIzvor, izvor.y / lenIzvor, izvor.z / lenIzvor, 1);

        ss >> Ia >> ka;
        ss >> Ii >> kd;
        
    }

    /* std::cout << "Ociste: " << glm::to_string(ociste) << std::endl;
    std::cout << "Glediste: " << glm::to_string(glediste) << std::endl; */

    std::ifstream filePoligon(argv[1]);
    bool first = true;

    if (filePoligon.is_open()) {
        std::string line;

        while(getline(filePoligon, line)) {
            std::string front;
            std::stringstream ss(line);

            if (line.front() == 'g') {
                ss >> front >> name;
            } else if (line.front() == 'v') {
                double x, y, z;
                ss >> front >> x >> y >> z;

                glm::dvec4 vrh = glm::dvec4(x, y, z, 1.0);

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
                vrhovi.push_back(vrh);
                
            } else if (line.front() == 'f') {
                int a, b, c;
                ss >> front >> a >> b >> c;
                glm::ivec3 trokut = glm::ivec3(a, b, c);

                trokuti.push_back(trokut);
                
            }
        }

        filePoligon.close();
    }

    srediste = glm::dvec4((xMax + xMin)/2, (yMax + yMin)/2,(zMax + zMin)/2, 1.0);

    for (int i = 0; i < vrhovi.size(); i++) {
        vrhovi.at(i).x = 2 * ((vrhovi.at(i).x - xMin) / (xMax - xMin)) - 1;
        vrhovi.at(i).y = 2 * ((vrhovi.at(i).y - yMin) / (yMax - yMin)) - 1;
        vrhovi.at(i).z = 2 * ((vrhovi.at(i).z - zMin) / (zMax - zMin)) - 1;
    }

    srediste = glm::dvec4(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < trokuti.size(); i++) {
        glm::ivec3 trokut = trokuti.at(i);
        glm::dvec3 dir = glm::cross(glm::dvec3(vrhovi.at(trokut[0] - 1) - vrhovi.at(trokut[1] - 1)), glm::dvec3(vrhovi.at(trokut[0] - 1) - vrhovi.at(trokut[2] - 1)));
        glm::dvec3 normala = dir / (sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
        normale.push_back(normala);
    }

    racunajTiP();

    /* bool unutarPoligona = true;

    for (int i = 0; i < brVrhova; i++) {
        if (glm::dot((glm::vec3)ociste, (glm::vec3)bridovi[i]) > 0) {
            unutarPoligona = false;
            break;
        }
    } */
    

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Vjezba 5");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutKeyboardFunc(myKeyboard);
	glutMainLoop();

}

void racunajTiP() {
    glm::mat4 T1, T2, T3, T4, T5;
    T1 = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(-ociste.x, -ociste.y, -ociste.z, 1));

    glm::vec4 G1 = glm::vec4(T1 * glediste);

    double sinA, cosA;
    sinA = G1.y / sqrt(pow(G1.x, 2) + pow(G1.y, 2));
    cosA = G1.x / sqrt(pow(G1.x, 2) + pow(G1.y, 2));

    T2 = glm::mat4(glm::vec4(cosA, -sinA, 0, 0), glm::vec4(sinA, cosA, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));

    glm::vec4 G2 = glm::vec4(T2 * G1);

    double sinB, cosB;
    sinB = G2.x / sqrt(pow(G2.x, 2) + pow(G2.z, 2));
    cosB = G1.z / sqrt(pow(G2.x, 2) + pow(G2.z, 2));

    T3 = glm::mat4(glm::vec4(cosB, 0, sinB, 0), glm::vec4(0, 1, 0, 0), glm::vec4(-sinB, 0, cosB, 0), glm::vec4(0, 0, 0, 1));

    glm::vec4 G3 = glm::vec4(T3 * G2);

    T4 = glm::mat4(glm::vec4(0, -1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
    
    T5 = glm::mat4(glm::vec4(-1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));

    T = T1 * T2 * T3 *T4 * T5;

    double H = G3.z;

    P = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 0, 1/H), glm::vec4(0, 0, 0, 0));
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
        glm::dvec3 normala = normale.at(i);

        double product = glm::dot(normala, glm::dvec3(ociste));



        if (product > 0) {
            double Id = Ia * ka + Ii * kd * glm::dot(normala, glm::dvec3(L));
            glColor3f((double)Id / 255, (double)fabs(0.5 - Id / 255), (double)(1 - Id / 255));

            glBegin(GL_TRIANGLES);
            {
                for (int j = 0; j < 3; j++) {
                    glm::vec4 vrh = vrhovi.at(trokut[j] - 1);
                    vrh = T * vrh;
                    vrh = P * vrh;
                    // std::cout << glm::to_string(vrh) << std::endl;
                    GLint x = (GLint) int((vrh.x + 1.0) * 100) + 100;
                    GLint y = (GLint) int((vrh.y + 1.0) * 100) + 100;
                    // std::cout << "x: " << x << ", y: " << y << std::endl;
                    glVertex2i(x, y);
                }
            }
            glEnd();
        }
        

    }

};

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    double promjena = 0;
	if (theKey == 'a' || theKey == 'b' || theKey == 'c' || theKey == 'x' || theKey == 'y' || theKey == 'z') {
        koordinata = theKey;
    } else if (theKey == '+') {
        promjena += 0.1;
    } else if (theKey == '-') {
        promjena -= 0.1;
    }

    if (promjena != 0) {
        switch (koordinata)
        {
        case 'a':
            ociste.x += promjena;
            break;
        case 'b':
            ociste.y += promjena;
            break;
        case 'c':
            ociste.z += promjena;
            break;
        case 'x':
            glediste.x += promjena;
            break;
        case 'y':
            glediste.y += promjena;
            break;
        case 'z':
            glediste.z += promjena;
            break;
        
        default:
            break;
        }
        racunajTiP();
	    display();
    }
};

