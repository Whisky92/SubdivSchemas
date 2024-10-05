#include "structures.h"
#include <iostream>

using namespace std;

int main() {
	
	Vertex v1(1.0, 2.0, 3.0);
	Vertex v2(2.0, 3.0, 4.0);
	Vertex v3(3.0, 4.0, 5.0);

	Face f1;
	HalfEdge h1(&v1, &f1);
	f1.halfEdge = &h1;
	cout << v1.toString() << endl;
	cout << f1.toString() << endl;
	cout << h1.toString() << endl;
	return 0;
}