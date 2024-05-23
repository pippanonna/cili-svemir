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

GLuint window;
GLuint width = 500, height = 500;
std::vector<glm::dvec4> kontrolniPoligon;
std::vector<int> b;

void reshape(int width, int height);
void display();
void renderScene();

int binomniKoef(int n, int k);

int main(int argc, char** argv) {
    std::ifstream file1(argv[1]);
    

    if (file1.is_open()) {
        std::stringstream ss;
        ss << file1.rdbuf();
        file1.close();

        double x, y;

        while(ss) {
            ss >> x >> y;
            kontrolniPoligon.push_back(glm::dvec4(x, y, 1, 1));

        }
        kontrolniPoligon.pop_back();
        
    }

    /* std::cout << "Tocke poligona: " << std::endl;
    for (int i = 0; i < kontrolniPoligon.size(); i++) {
        std::cout << "T" << i + 1 << ": " << glm::to_string(kontrolniPoligon.at(i)) << std::endl;

    } */

    for (int i = 0; i < kontrolniPoligon.size(); i++) {
        int koef = binomniKoef(kontrolniPoligon.size() - 1, i);
        // std::cout << koef << std::endl;
        b.push_back(koef);
    }

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
	glutCreateWindow("Vjezba 6");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

}

int binomniKoef(int n, int k) {
   if (k == 0 || k == n)
   return 1;
   return binomniKoef(n - 1, k - 1) + binomniKoef(n - 1, k);
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
	glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
	{
        for (int i = 0; i < kontrolniPoligon.size() - 1; i++) {
            glVertex2i((GLint) kontrolniPoligon.at(i)[0], (GLint) height - kontrolniPoligon.at(i)[1]);
            glVertex2i((GLint) kontrolniPoligon.at(i + 1)[0], (GLint) height - kontrolniPoligon.at(i + 1)[1]);
        }
	}
	glEnd();

    glBegin(GL_LINE_STRIP);
    for (double t = 0.0; t <= 1.0; t += 0.01) {
        double x = 0, y = 0;
        int n = kontrolniPoligon.size();
        for (int i = 0; i < n; i++) {
            x += b[i] * pow(t, i) * pow(1 - t, n - 1 - i) * kontrolniPoligon[i].x;
            y += b[i] * pow(t, i) * pow(1 - t, n - 1 - i)* kontrolniPoligon[i].y;
        }
        // std::cout << x << " " << y << std::endl;
        glVertex2i((GLint) (int) x, (GLint) height - (int) y);
    }
	glEnd();

};

