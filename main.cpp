#include "structures.h"
#include <iostream>

using namespace std;

int main() {

	ObjectModel objectModel = ObjectModel();

	//bool ok = objectModel.readObjFile("resources/sphere.obj");
	bool ok = objectModel.readObjFile("resources/tetrahedron.obj");

	std::vector<std::vector<Vertex*>> triangles = objectModel.getTriangles();
	for (std::vector<Vertex*> vertices : triangles) {
		for (Vertex* vertex : vertices)
			std::cout << vertex->toString() << '\t';
		std::cout << '\n';
	}

	return 0;
}