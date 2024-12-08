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
	Vertex();
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
	HalfEdge();
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
public:
	bool readObjFile(std::string filename);
	Vertex* minCubePos;
	Vertex* maxCubePos;

	ObjectModel();
	~ObjectModel();
	std::vector<std::vector<Vertex*>> getTriangles();
	std::vector<Vertex*> doLoopSubdivision();
	void freeMemory();
private:
	std::vector<Vertex*> vertices;
	std::vector<Face*> faces;
	std::vector<HalfEdge*> halfEdges;

	int getHalfEdgeIndex(HalfEdge& halfEdge);
	Vertex* createVertexAtHalfWay(Vertex& v1, Vertex& v2);
	Vertex* createVertexWithWeights(std::vector<Vertex*>& vertecis, std::vector<float>& weights);
};

#endif