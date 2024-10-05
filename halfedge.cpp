#include "structures.h"

HalfEdge::HalfEdge(Vertex* origin, Face* incidentFace, HalfEdge* next, HalfEdge* prev) {
	this->origin = origin;
	this->incidentFace = incidentFace;
	this->next = next;
	this->prev = prev;
}

HalfEdge::HalfEdge(Vertex* origin, Face* incidentFace) {
	this->origin = origin;
	this->incidentFace = incidentFace;
}

std::string HalfEdge::toString() {
	return "HalfEdge (" + this->origin->toString() + ")";
}
