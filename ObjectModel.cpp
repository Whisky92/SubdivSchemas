#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "structures.h"

ObjectModel::ObjectModel() {};

bool ObjectModel::readObjFile(std::string filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::string command;
    float xPos, yPos, zPos;
    std::string pos1, pos2, pos3;
    std::vector<std::vector<int>> faceIndecis;
    Vertex* minCubePos = new Vertex(1000000, 1000000, 1000000);
    Vertex* maxCubePos = new Vertex(-1000000, -1000000, -1000000);
    while (!file.eof()) {
        file >> command;
        if (command == "v") {
            file >> xPos >> yPos >> zPos;
            this->vertices.push_back(Vertex(xPos, yPos, zPos));
            if (minCubePos->x > xPos)
                minCubePos->x = xPos;
            if (minCubePos->y > yPos)
                minCubePos->y = yPos;
            if (minCubePos->z > zPos)
                minCubePos->z = zPos;

            if (maxCubePos->x < xPos)
                maxCubePos->x = xPos;
            if (maxCubePos->y < yPos)
                maxCubePos->y = yPos;
            if (maxCubePos->z < zPos)
                maxCubePos->z = zPos;
        }
        if (command == "f") {
            file >> pos1 >> pos2 >> pos3;
            int indexOfVertex1 = std::stoi(pos1.substr(0, pos1.find('/', 0)));
            int indexOfVertex2 = std::stoi(pos2.substr(0, pos2.find('/', 0)));
            int indexOfVertex3 = std::stoi(pos3.substr(0, pos3.find('/', 0)));
            std::vector<int> currentFace(3);
            currentFace[0] = indexOfVertex1;
            currentFace[1] = indexOfVertex2;
            currentFace[2] = indexOfVertex3;
            faceIndecis.push_back(currentFace);
        }
    }
    file.close();

    this->minCubePos = minCubePos;
    this->maxCubePos = maxCubePos;

    for (auto& currentFaceIndecis : faceIndecis) {
        Face* currentFace = new Face();

        std::vector<HalfEdge*> currentHalfEdges = {
            new HalfEdge(),
            new HalfEdge(),
            new HalfEdge()
        };

        for (int i = 0; i < currentHalfEdges.size(); i++) {
            currentHalfEdges[i]->origin = &this->vertices[currentFaceIndecis[i]-1];
            currentHalfEdges[i]->incidentFace = currentFace;
            currentHalfEdges[i]->next = currentHalfEdges[(i + 1) % currentHalfEdges.size()];
            currentHalfEdges[i]->prev = currentHalfEdges[(i + 2) % currentHalfEdges.size()];
            for (auto& he : this->halfEdges) {
                if (he.origin == &this->vertices[currentFaceIndecis[(i+1) % 3] - 1] && he.next->origin == currentHalfEdges[i]->origin) {
                    if (he.twin != nullptr) {
                        std::cout << "Something went wrong: " << he.toString() << std::endl;
                    }
                    he.twin = currentHalfEdges[i];
                    currentHalfEdges[i]->twin = &he;
                    break;
                }
            }
        }
        for (auto& he : currentHalfEdges) {
            this->halfEdges.push_back(*he);
        }
        currentFace->halfEdge = currentHalfEdges[0];
        this->faces.push_back(*currentFace);
    }

	return true;
}

std::vector<std::vector<Vertex*>> ObjectModel::getTriangles() {
    std::vector<std::vector<Vertex*>> triangles(
        this->faces.size(),
        std::vector<Vertex*>(3));
    for (int i = 0; i < this->faces.size(); i++) {
        HalfEdge* halfEdge = this->faces[i].halfEdge;
        std::vector<Vertex*> vertices(3);
        for (int i = 0; i < 3; i++) {
            vertices[i] = halfEdge->origin;
            halfEdge = halfEdge->next;
        }
        triangles[i] = vertices;
    }
    return triangles;
}

int ObjectModel::getHalfEdgeIndex(HalfEdge& halfEdge) {
    for (int i = 0; i < this->halfEdges.size(); i++) {
        if (&this->halfEdges[i] == &halfEdge) {
            return i;
        }
    }
    return -1;
}

std::vector<Vertex*> ObjectModel::doLoopSubdivision() {
    std::vector<Vertex*> oddVertecis = std::vector<Vertex*>(this->halfEdges.size(), nullptr);
    std::vector<float> loopSubdivScheme = { 3.0 / 8.0, 3.0 / 8.0, 1.0 / 8.0, 1.0 / 8.0 };

    for (int i = 0; i < this->halfEdges.size(); i++) {
        if (oddVertecis[i] == nullptr) {
            std::vector<Vertex*> vertecis = {
                this->halfEdges[i].origin,                  // halfedge origin
                this->halfEdges[i].next->origin,            // halfedge destination
                this->halfEdges[i].next->next->origin,      // third vertex of face
                this->halfEdges[i].twin->next->next->origin // twin's farer vertex
            };
            Vertex* currentOddVertex = this->createVertexWithWeights(vertecis, loopSubdivScheme);

            /*Vertex* originVertex = this->halfEdges[i].origin;
            Vertex* nextVertex = this->halfEdges[i].next->origin;
            Vertex* currentOddVertex = this->createVertexAtHalfWay(*originVertex, *nextVertex);*/
            oddVertecis[i] = currentOddVertex;
            oddVertecis[this->getHalfEdgeIndex(*this->halfEdges[i].twin)] = currentOddVertex;
        }
    }

    return oddVertecis;
}

Vertex* ObjectModel::createVertexAtHalfWay(Vertex& v1, Vertex& v2) {
    Vertex* oddVertex = new Vertex(
        (v1.x + v2.x) / 2.0,
        (v1.y + v2.y) / 2.0,
        (v1.z + v2.z) / 2.0
    );

    return oddVertex;
}


Vertex* ObjectModel::createVertexWithWeights(std::vector<Vertex*> &vertecis, std::vector<float> &weights) {
    while (vertecis.size() > weights.size())
        weights.push_back(1.0);

    Vertex* oddVertex = new Vertex();
    for (int i = 0; i < vertecis.size(); i++)
    {
        oddVertex->x += vertecis[i]->x * weights[i];
        oddVertex->y += vertecis[i]->y * weights[i];
        oddVertex->z += vertecis[i]->z * weights[i];
    }

    return oddVertex;
};
