#define _USE_MATH_DEFINES 

#include "structures.h"
#include <iostream>
#include <array>
#include <fstream>

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

const static std::string sourceFile = "resorces/sphere.obj";

static ObjectModel objectModel = ObjectModel();
static bool isSuccessfulRead;

static float radiusX, radiusY, radiusZ;
static float frustumBottom, frustumTop, frustumLeft, frustumRight, frustumNear, frustumFar;                    
static float camX, camY, camZ;
static float centerX, centerY, centerZ;
static float angleX = 0.0, angleY = 0.0, angleZ = 0.0;
static const float rotateStep = 4.0;

void calculateViewPositions() {
	Vertex* minPos = objectModel.minCubePos;
	Vertex* maxPos = objectModel.maxCubePos;
	centerX = (minPos->x + maxPos->x) / 2;
	centerY = (minPos->y + maxPos->y) / 2;
	centerZ = (minPos->y + maxPos->z) / 2;

	radiusX = maxPos->x - centerX;
	radiusY = maxPos->y - centerY;
	radiusZ = maxPos->z - centerZ;
	
	float zOffset = radiusZ * 3.0;

	float frustrumSize = std::max(radiusX, std::max(radiusY, radiusZ));
	float multiplier = 1.5;

	frustumBottom = -frustrumSize * multiplier;
	frustumTop = frustrumSize * multiplier;
	frustumLeft = -frustrumSize * multiplier;
	frustumRight = frustrumSize * multiplier;
	frustumNear = frustrumSize;
	frustumFar = frustumNear * 6;

	camX = centerX;
	camY = centerY;
	camZ = (frustumNear - maxPos->z - zOffset) < radiusZ ? maxPos->z + radiusZ + zOffset : frustumNear;

	std::cout << "centerZ " << centerZ << std::endl;
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
	std::cout << "centerX " << centerX << std::endl;
	std::cout << "centerY " << centerY << std::endl;
	std::cout << "centerZ " << centerZ << std::endl;
}

std::array<float, 3> calculateNormalVertices(std::vector<Vertex*> vertices) {

	float ux = vertices[1]->x - vertices[0]->x;
	float uy = vertices[1]->y - vertices[0]->y;
	float uz = vertices[1]->z - vertices[0]->z;

	float vx = vertices[2]->x - vertices[0]->x;
	float vy = vertices[2]->y - vertices[0]->y;
	float vz = vertices[2]->z - vertices[0]->z;

	float nx = uy * vz - uz * vy;
	float ny = uz * vx - ux * vz;
	float nz = ux * vy - uy * vx;

	float length = sqrt(nx * nx + ny * ny + nz * nz);
	if (length > 0.0f) {
		nx /= length;
		ny /= length;
		nz /= length;
	}

	return { nx, ny, nz };
}


void drawTriangles() {
	std::vector<std::vector<Vertex*>> triangles = objectModel.getTriangles();
	// std::vector<Vertex*> oddVertices = objectModel.doLoopSubdivision();

	for (std::vector<Vertex*> vertices : triangles) {
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(0.4f, 0.416f, 0.427f);
		glBegin(GL_TRIANGLES);

		std::array<float, 3> norms = calculateNormalVertices(vertices);
		for (Vertex* vertex : vertices) {
			glNormal3f(norms[0], norms[1], norms[2]);
			glVertex3f(vertex->x, vertex->y, vertex->z);
		}
		glEnd();

		glPolygonMode(GL_FRONT, GL_LINE);

		if (sourceFile == "resources/tetrahedron.obj") {
			glLineWidth(3.0f);
		}
		else {
			glLineWidth(2.0f);
		}

		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_LOOP);
		for (Vertex* vertex : vertices) {
			glVertex3f(vertex->x, vertex->y, vertex->z);
		}
		glEnd();
	}

	/* glPointSize(5.0f);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	for (Vertex* vertex : oddVertices) {
		glVertex3f(vertex->x, vertex->y, vertex->z);
	}
	glEnd(); */
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(camX, camY, camZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

	glTranslatef(centerX, centerY, centerZ);

	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 0.0, 0.0, 1.0);

	glTranslatef(-centerX, -centerY, -centerZ);

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
	glOrtho(frustumLeft, frustumRight, frustumBottom, frustumTop, frustumNear, frustumFar);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void keyInput(unsigned char key, int x, int y)
{	
	switch (key)
	{
		case 'x':
			angleX += rotateStep;
			if (angleX > 360.0) angleX -= 360.0;
			break;
		case 'X':
			angleX -= rotateStep;
			if (angleX <= 0.0) angleX += 360.0;
			break;
		case 'y':
			angleY += rotateStep;
			if (angleY > 360.0) angleY -= 360.0;
			break;
		case 'Y':
			angleY -= rotateStep;
			if (angleY <= 0.0) angleY += 360.0;
			break;
		case 'z':
			angleZ += rotateStep;
			if (angleZ > 360.0) angleZ -= 360.0;
			break;
		case 'Z':
			angleZ -= rotateStep;
			if (angleZ <= 0.0) angleZ += 360.0;
			break;
		case 's':
			objectModel.doLoopSubdivision();
			break;
		case 'q':
			exit(0);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void setupLighting(void) {
	// Enable the lighting system
	glEnable(GL_LIGHTING);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.4, 0.4, 0.4, 1.0 };

	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

	// Material property vectors.
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 20.0f };

	// Material properties shared by all the spheres.
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

	// Cull back faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable color material mode:
	// The ambient and diffuse color of the front faces will track the color set by glColor().
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void setup(void)
{
	isSuccessfulRead = objectModel.readObjFile(sourceFile);
	calculateViewPositions();
	setupLighting();
	glClearColor(0.933f, 0.804f, 0.561f, 1.0f);
}


// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) camZ+=.5;
	else if (key == GLUT_KEY_DOWN) camZ-=.5;
	glutPostRedisplay();
}

void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << std::endl;
	std::cout << "Press X to rotate the object around X axis" << std::endl;
	std::cout << "Press Y to rotate the object around Y axis" << std::endl;
	std::cout << "Press Z to rotate the object around Z axis" << std::endl;
}

int main(int argc, char** argv)
{
	std::ifstream f(sourceFile.c_str());
	if (!f.good()) {
		std::cout << "Invalid file" << std::endl;
		return 0;
	}

	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
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