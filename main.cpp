#define _USE_MATH_DEFINES 

#include "structures.h"
#include <iostream>

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

// Globals.
static float camX = 0.0, camY = 0.0, camZ = 5.0;
static float* camToMove = &camZ;

void drawTriangles() {
	ObjectModel objectModel = ObjectModel();
	bool isSuccessfulRead = objectModel.readObjFile("resources/sphere.obj");

	if (isSuccessfulRead) {
		std::vector<std::vector<Vertex*>> triangles = objectModel.getTriangles();

		for (std::vector<Vertex*> vertices : triangles) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_TRIANGLES);
			for (Vertex* vertex : vertices) {
				glVertex3f(vertex->x, vertex->y, vertex->z);
			}
			glEnd();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_TRIANGLES);
			for (Vertex* vertex : vertices) {
				glVertex3f(vertex->x, vertex->y, vertex->z);
			}
			glEnd();
		}
	}
}

void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	drawTriangles();

	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 20.0); //(-5.0, -5.0, -5.0) (5.0, 5.0, -5.0) (-20.0, -20.0, -20.0) (20.0, 20.0, -20.0)
	glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'x':
			camToMove = &camX;
			break;
		case 'y':
			camToMove = &camY;
			break;
		case 'z':
			camToMove = &camZ;
			break;
		default:
			break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP:
			*camToMove += 0.5;
			break;
		case GLUT_KEY_DOWN:
			*camToMove -= 0.5;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << std::endl;
	std::cout << "Press X, Y or Z to select the camera position to be changed! (Default is Z)" << std::endl;
	std::cout << "Press the UP and DOWN arrow keys to increase and decrease the selected camera position respectively!" << std::endl;

}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("subdiv.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}