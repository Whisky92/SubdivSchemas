#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>

class Face;

class Vertex {

public:
	float x;
	float y;
	float z;

	Vertex(float x, float y, float z);
	bool isEqual(const Vertex& Ref) const;
	std::string toString();
};

class HalfEdge {

public:
	Vertex* origin;
	HalfEdge* twin;
	Face* incidentFace;
	HalfEdge* next;
	HalfEdge* prev;

	HalfEdge(Vertex* origin, Face* incidentFace, HalfEdge* next, HalfEdge* prev);
	HalfEdge(Vertex* origin, Face* incidentFace);
	std::string toString();
};

class Face {

public:
	HalfEdge* halfEdge;

	Face();
	Face(HalfEdge* halfEdge);
	std::string toString();
};

class ObjectModel {
	std::vector<Vertex> vertices;
	std::vector<Face> faces;
	std::vector<HalfEdge> halfEdges;
public:
	ObjectModel();
	bool readObjFile(std::string filename);
	HalfEdge getHalfEdgeByOrigin(Vertex& vertex);
};

#endif