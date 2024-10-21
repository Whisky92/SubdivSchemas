#define _USE_MATH_DEFINES 

#include "structures.h"
#include <iostream>

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

const static std::string sourceFile = "resources/tetrahedron.obj";

static ObjectModel objectModel = ObjectModel();
static bool isSuccessfulRead;

static float distanceZ;
static float radiusX, radiusY, radiusZ;
static float frustumBottom, frustumTop, frustumLeft, frustumRight, frustumNear, frustumFar;                    
static float camX, camY, camZ;
static float centerX, centerY, centerZ;
static float angle = 0;

void calculateViewPositions() {
	Vertex* minPos = objectModel.minCubePos;
	Vertex* maxPos = objectModel.maxCubePos;
	float shapeCenterX = (minPos->x + maxPos->x) / 2;
	float shapeCenterY = (minPos->y + maxPos->y) / 2;
	float shapeCenterZ = (minPos->y + maxPos->z) / 2;

	radiusX = maxPos->x - shapeCenterX;
	radiusY = maxPos->y - shapeCenterY;
	radiusZ = (maxPos->z - shapeCenterZ);
	
	float zOffset = radiusZ * 3.0;

	frustumBottom = minPos->y - radiusY;
	frustumTop = maxPos->y + radiusY;
	frustumLeft = minPos->x - radiusX;
	frustumRight = maxPos->x + radiusX;
	frustumNear = (zOffset - maxPos->z) > radiusZ ? maxPos->z + radiusZ + zOffset : zOffset;
	//frustumNear = maxPos->z > radiusZ ? maxPos->z + radiusZ + 0.0 : 0.0;
	frustumFar = frustumNear * 5;

	centerX = shapeCenterX;
	centerY = shapeCenterY;
	centerZ = shapeCenterZ;

	camX = centerX;
	camY = centerY;
	camZ = (frustumNear - maxPos->z - zOffset) < radiusZ ? maxPos->z + radiusZ + zOffset : frustumNear;

	distanceZ = camZ - shapeCenterZ;

	std::cout << "shapeCenterZ " << shapeCenterZ << std::endl;
	std::cout << "maxX " << maxPos->x << std::endl;
	std::cout << "maxY " << maxPos->y << std::endl;
	std::cout << "maxZ " << maxPos->z << std::endl;
	std::cout << "radiusX " << radiusX << std::endl;
	std::cout << "radiusY " << radiusY << std::endl;
	std::cout << "radiusZ " << radiusZ << std::endl;
	std::cout << "frustumTop " << frustumTop << std::endl;
	std::cout << "frustumBottom " << frustumBottom << std::endl;
	std::cout << "frustumRight " << frustumRight << std::endl;
	std::cout << "frustumLeft " << frustumLeft << std::endl;
	std::cout << "frustumNear " << frustumNear << std::endl;
	std::cout << "frustumFar " << frustumFar << std::endl;
	std::cout << "camX " << camX << std::endl;
	std::cout << "camY " << camY << std::endl;
	std::cout << "camZ " << camZ << std::endl;
	std::cout << "distanceZ " << distanceZ << std::endl;
}

void drawTriangles() {
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

void setup(void)
{
	isSuccessfulRead = objectModel.readObjFile(sourceFile);
	calculateViewPositions();
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	camZ = centerZ + distanceZ * cos(angle);
	camY = centerY + distanceZ * sin(angle);

	std::cout << std::endl;
	std::cout << "camZ: " << camZ << std::endl;
	std::cout << "camY: " << camY << std::endl;
	std::cout << "distanceZ: " << distanceZ << std::endl;
	std::cout << "angle: " << angle << std::endl;
	gluLookAt(camX, camY, camZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

	if (isSuccessfulRead)
	{
		drawTriangles();
	}

	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(frustumLeft, frustumRight, frustumBottom, frustumTop, frustumNear, frustumFar); //(-5.0, -5.0, -5.0) (5.0, 5.0, -5.0) (-20.0, -20.0, -20.0) (20.0, 20.0, -20.0)
	glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'r':
			angle += 2;
			if (angle == 360) {
				angle = 0;
			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << std::endl;
	std::cout << "Press R to rotate the camera around the object" << std::endl;

}

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

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}