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
GLuint width, height;
std::vector<glm::ivec3> vrhovi;
std::vector<glm::ivec3> bridovi;
glm::ivec3 tocka;
int brVrhova;
int xMin, xMax, yMin, yMax;

void reshape(int width, int height);
void display();
void renderScene();
std::vector<glm::ivec3> racunajBridove(std::vector<glm::ivec3> arr, int n);


int main(int argc, char** argv) {
    std::ifstream file("prPol.txt");
    

    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        ss >> brVrhova;

        for (int i = 0; i < brVrhova; i++) {
            int x, y;
            ss >> x >> y;

            if (i == 0) {
                xMin = xMax = x;
                yMin = yMax = y;
            } else {
                if (xMin > x) {
                    xMin = x;
                }
                if (xMax < x) {
                    xMax = x;
                }
                if (yMin > y) {
                    yMin = y;
                }
                if (yMax < y) {
                    yMax = y;
                }
            }

            vrhovi.push_back(glm::ivec3(x, y, 1));
        }

        glm::ivec3 vector = glm::ivec3(vrhovi.front().x, vrhovi.front().y, 1);
        vrhovi.push_back(vector);

        int x, y;
        ss >> x >> y;
        tocka = glm::ivec3(x, y, 1);
        
    }

    if (tocka.x > xMax) width += tocka.x;
    else width += xMax;

    width += 50;

    if (tocka.y > yMax) height += tocka.y;
    else height += yMax;

    if (tocka.y < yMin) height -= tocka.y;
    else height -= yMin;
    
    height += 50;

    for (int i = 0; i < brVrhova; i++) {
        int a = 0, b = 0, c = 0;
        /* a = arr[i].y - arr[i+1].y;
        b =  arr[i+1].x - arr[i].x;
        c = arr[i].x * arr[i+1].y - arr[i+1].x * arr[i].y; */

        glm::dvec3 br = glm::cross(glm::dvec3(vrhovi[i]), glm::dvec3(vrhovi[i + 1]));
        a = br.x; b = br.y; c = br.z;

        bridovi.push_back(glm::ivec3(a, b, c));

    }

    /* bool uSmjeruSuprotno = true;
    for (int i = 0; i < bridovi.size(); i++) {
        if (glm::dot((glm::vec3)*bridovi.at(i), (glm::vec3)vrhovi.at((i + 2) % brVrhova)) < 0) {
            uSmjeruSuprotno = false;
        } 
    }

    if(uSmjeruSuprotno) {
        printf("Koordinate su zadane u smjeru suprotno od kazaljke\n");
    } else {
        printf("Koordinate nisu zadane u smjeru suprotno od kazaljke\n");

    } */
    
    printf("Broj vrhova poligona: %17d\n", brVrhova);
    for (int i = 0; i < brVrhova; i++) {
        printf("Koordinate vrha %d: %20d %5d\n", i + 1, vrhovi.at(i)[0], vrhovi.at(i)[1]);
    }
    printf("Koordinate ispitne tocke: %13d %5d\n", tocka[0], tocka[1]);


    /* for (int i = 0; i < brVrhova; i++) {
        printf("Brid %d: a:%5d b:%5d c:%7d\n", i + 1, bridovi.at(i)[0], bridovi.at(i)[1], bridovi.at(i)[2]);
    } */

    bool unutarPoligona = true;

    for (int i = 0; i < brVrhova; i++) {
        if (glm::dot((glm::vec3)tocka, (glm::vec3)bridovi[i]) > 0) {
            unutarPoligona = false;
            break;
        }
    }

    printf("TOCKA V JE ");
    if (unutarPoligona) {
        printf("UNUTAR POLIGONA!\n");
    } else {
        printf("IZVAN POLIGONA!\n");
    }

    height = 500; width = 500;


    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Vjezba 3");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

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
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	{
        for (int i = 0; i < brVrhova; i++) {
            glVertex2i((GLint) vrhovi.at(i)[0], (GLint) height - vrhovi.at(i)[1]);
            glVertex2i((GLint) vrhovi.at(i + 1)[0], (GLint) height - vrhovi.at(i + 1)[1]);
        }
	}
	glEnd();
    for (int y = yMin; y <= yMax; y++) {
        int L = xMin, D = xMax, x1;
        for (int i = 0; i < brVrhova; i++) {
            glm::ivec3 vec = bridovi.at(i);
            if (vec.x == 0) continue;
            x1 = (int)(- vec.y * y - vec.z) / (int)vec.x;
            if ((bridovi.at(i))[0] < 0 && x1 > L) {
                L = x1;
            } else if ((bridovi.at(i))[0] >= 0 && x1 < D) {
                D = x1;
            }

        }
        if (L < D) {
            glBegin(GL_LINES);
            {
                glVertex2i((GLint) L, (GLint) height - y);
                glVertex2i((GLint) D, (GLint) height - y);
            }
            glEnd();
            
        }
    }
    glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(5.0f);
    glBegin(GL_POINTS);
    {
        glVertex2i((GLint) tocka[0], (GLint) height - tocka[1]);
    }
	glEnd();



};
