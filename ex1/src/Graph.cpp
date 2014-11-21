/*
 * Graph.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: ixi
 */

#include "Graph.h"
#include <iostream>
#include <sstream>

namespace tcbvrp {

Graph::Graph() {

}

Graph::~Graph() {

}

void Graph::addNode(Node* node) {
	nodes.push_back(node);

	switch (node->getType()) {
	case Node::Type::ZERO:
		zeroNode = node;
		break;
	case Node::Type::SUPPLY:
		supplyNodes.push_back(node);
		break;
	case Node::Type::DEMAND:
		demandNodes.push_back(node);
		break;
	}
}

void Graph::setAdjacencyMatrix(std::vector< std::vector<int> > adjacencyMatrix) {

}

int Graph::getNumberOfNodes() {
	return nodes.size();
}

/**
 * Reads given textfile and creates Graph objects as specified in the file.
 */
static Graph createGraph(std::string filename) {

	std::ifstream input(filename.c_str());
	std::string line;
	int linenr = 0;
	std::string ref = "0123456789";
	Graph graph;
	std::vector< std::vector<int> > adjacencyMatrix;

	int numNodes = -1;
	int numVehicles = -1;
	int globalTimeLimit = -1;

	Node *zero = new Node(Node::Type::ZERO, 0);
	graph.addNode(zero);

	while (std::getline(input, line)) {
		linenr++;

		if (ref.find(line[0]) == std::string::npos) {
			// Not a graph line (must start with a number)
			continue;
		}
		else {
			std::istringstream converter(line);

			if (numNodes == -1) {
				converter >> numNodes;
			}
			else if (numVehicles == -1) {
				converter >> numVehicles;
			}
			else if (globalTimeLimit == -1) {
				converter >> globalTimeLimit;
			}
			else if (numNodes-1 > 0) {
				char type;
				int id;
				Node *node;

				converter >> id;
				converter >> type;

				switch (type) {
				case 'S':
					node = new Node(Node::Type::SUPPLY, id);
					break;
				case 'D':
					node = new Node(Node::Type::DEMAND, id);
					break;
				default 'D':
					std::cout << linenr << ": Invalid node type." << std::endl;
				}

				graph.addNode(node);

				// Count down all Nodes that are read
				numNodes--;
			}
			else if (numNodes == 0) {

				int weight;
				for (int i=0; i<numNodes; i++) {
					std::vector<int> row;

					for (int j=0; j<numNodes; j++) {

						converter >> weight;
						row.push_back(weight);
					}
					adjacencyMatrix.push_back(row);
				}
				graph.setAdjacencyMatrix(adjacencyMatrix);
			}

			if (converter.fail()) {
				std::cout << linenr << ": Invalid input format." << std::endl;
			}


		}
	}

	return graph;
}


} /* namespace tcbvrp */
