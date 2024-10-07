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
    while (!file.eof()) {
        file >> command;
        if (command == "v") {
            file >> xPos >> yPos >> zPos;
            this->vertices.push_back(Vertex(xPos, yPos, zPos));
        }
        if (command == "f") {
            file >> pos1 >> pos2 >> pos3;
            pos1 = pos1.substr(0, pos1.find('/', 0));
            pos2 = pos2.substr(0, pos2.find('/', 0));
            pos3 = pos3.substr(0, pos3.find('/', 0));
        }
    }

    file.close();
	return true;
}

HalfEdge ObjectModel::getHalfEdgeByOrigin(Vertex& vertex) {
    for (auto halfEdge : this->halfEdges) {
        if (halfEdge.origin->isEqual(vertex)) {
            return halfEdge;
        }
    }
}

