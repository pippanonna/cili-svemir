#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <cmath>

GLuint window;
GLuint width = 500, height = 500;

void reshape(int width, int height);
void display();
void renderScene();

double uMin, uMax, vMin, vMax, xMax, yMax, cReal, cImag;
int eps, m;
bool julia;

int testDivergencije(double u0, double v0, bool isJulia) {
    
    double zReal = 0, zImag = 0, r = 0;
    int k = 0;

    if (isJulia) {
        zReal = u0; 
        zImag = v0;
        r = pow(zReal, 2) + pow(zImag, 2);
        if (r > eps) {
            return 0;
        } 
    } else {
        cReal = u0;
        cImag = v0;
    }

    do {
        k++;
        double zNextReal, zNextImag;
        zNextReal = pow(zReal, 2) - pow(zImag, 2) + cReal;
        zNextImag = 2 * zReal * zImag + cImag;
        zReal = zNextReal;
        zImag = zNextImag;

        r = sqrt(pow(zReal, 2) + pow(zImag, 2));
    } while(k < m && r < eps);

    return k;
}

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    

    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        ss >> julia;

        ss >> eps >> m;
        ss >> uMin >> uMax;
        ss >> vMin >> vMax;
        ss >> xMax >> yMax;

        if (julia) {
            ss >> cReal >> cImag;
        }

        width = xMax;
        height = yMax;
        
    }

    if (julia) {
        double eps2 = cReal * cReal + cImag * cImag;
        if (eps2 > eps) eps = eps2;
    }
    

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Vjezba 8");
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
	glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_POINTS);
	{
        for (int x0 = 0; x0 < width; x0++) {
            for (int y0 = 0; y0 < height; y0++) {
                double u0, v0;
                u0 = x0 / (double)(xMax)*(uMax - uMin) + uMin;
                v0 = y0 / (double)(yMax)*(vMax - vMin) + vMin;

                int boja;
                boja = testDivergencije(u0, v0, julia);

                if (boja >= m) {
                    glColor3f(0.0f, 0.0f, 0.0f);
                } else {
                    glColor3f((double)boja/m, (double)fabs(0.5 - boja/m), (double)(1 - boja/m));
                }
                glVertex2i(x0, y0);

            }
        }
	}
	glEnd();

};

