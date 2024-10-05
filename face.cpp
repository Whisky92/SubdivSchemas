#include "structures.h"

Face::Face() {}

Face::Face(HalfEdge* halfEdge) {
	this->halfEdge = halfEdge;
}

std::string Face::toString() {
	if (this->halfEdge == nullptr) {
		return "Face()";
	} else {
		return "Face( " + this->halfEdge->origin->toString() + ")";
	}
}
