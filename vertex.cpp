#include "structures.h"

Vertex::Vertex(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
};

std::string Vertex::toString() {
	return "Vertex (" + std::to_string(this->x) + " " + std::to_string(this->y) + " " + std::to_string(this->z) + ")";
}

bool Vertex::isEqual(const Vertex& Ref) const {
	return (this->x == Ref.x && this->y == Ref.y && this->z == Ref.z);
}
