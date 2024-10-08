#include "structures.h"
#include <iostream>

using namespace std;

int main() {

	ObjectModel objectModel = ObjectModel();

	bool ok = objectModel.readObjFile("resources/sphere.obj");

	return 0;
}