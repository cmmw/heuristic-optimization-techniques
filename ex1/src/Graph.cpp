/*
 * Graph.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: ixi
 */

#include "Graph.h"
#include <iostream>
#include <sstream>
#include <fstream>


namespace tcbvrp {

Graph::Graph() : globalTimeLimit(-1), numberOfVehicles(-1), zeroNode(0) {

}

Graph::~Graph() {

}

void Graph::addNode(Node* node) {
	nodes.push_back(node);

	switch (node->getType()) {
	case Node::ZERO:
		zeroNode = node;
		break;
	case Node::SUPPLY:
		supplyNodes.push_back(node);
		break;
	case Node::DEMAND:
		demandNodes.push_back(node);
		break;
	}
}

int Graph::getNumberOfNodes() {
	return nodes.size();
}

/**
 * Reads given textfile and creates Graph objects as specified in the file.
 */
Graph Graph::createGraph(std::string filename) {

	std::ifstream input(filename.c_str());
	std::string line;
	int linenr = 0;
	std::string ref = "0123456789";
	Graph graph;
	std::vector< std::vector<int> > adjacencyMatrix;

	int numNodes = -1;
	int count = -1;
	int count2 = -1;
	int numVehicles = -1;
	int globalTimeLimit = -1;

	Node *zero = new Node(Node::ZERO, 0);
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
				count = numNodes;
				count2 = numNodes+1;
			}
			else if (globalTimeLimit == -1) {
				converter >> globalTimeLimit;
				graph.setGlobalTimeLimit(globalTimeLimit);

			}
			else if (numVehicles == -1) {
				converter >> numVehicles;
				graph.setNumberOfVehicles(numVehicles);

			}
			else if (count > 0) {
				char type;
				int id;
				Node *node;

				converter >> id;
				converter >> type;

				switch (type) {
				case 'S':
					node = new Node(Node::SUPPLY, id);
					break;
				case 'D':
					node = new Node(Node::DEMAND, id);
					break;
				default:
					std::cout << linenr << ": Invalid node type." << std::endl;
				}

				graph.addNode(node);

				// Count down all Nodes that are read
				count--;
			}
			else if (count == 0 && count2 > 0) {

				int weight;
				std::vector<int> row;

				for (int i=0; i<numNodes+1; i++) {

					converter >> weight;
					row.push_back(weight);
				}
				adjacencyMatrix.push_back(row);

				// Count down all matrix lines that are read
				count2--;
			}

			graph.setAdjacencyMatrix(adjacencyMatrix);

			if (converter.fail()) {
				std::cout << linenr << ": Invalid input format." << std::endl;
			}

		}
	}


	return graph;
}

void Graph::printGraph() {
	bool printTextDescription = true;
	bool printZeroNode = true;
	int numNodes = getNumberOfNodes();

	if (printTextDescription) {
		std::cout << "Number of nodes: " << std::endl;
	}
	std::cout << numNodes << std::endl;

	if (printTextDescription) {
		std::cout << "Global time limit: " << std::endl;
	}
	std::cout << getGlobalTimeLimit() << std::endl;

	if (printTextDescription) {
		std::cout << "Number of vehicles: ";
	}
	std::cout << getNumberOfVehicles() << std::endl;

	if (printTextDescription) {
		std::cout << "Nodes: " << std::endl;
	}

	for (int i=0; i<numNodes; i++) {
		if (i==0 && !printZeroNode) {
			continue;
		}
		std::string type;

		switch (nodes.at(i)->getType()) {
		case Node::ZERO:
			type = "Z";
			break;
		case Node::SUPPLY:
			type = "S";
			break;
		case Node::DEMAND:
			type = "D";
			break;
		}

		std::cout << i << " " << type << std::endl;
	}

	if (printTextDescription) {
		std::cout << "Adjacency Matrix: " << std::endl;
	}
	for (int i=0; i<numNodes; i++) {
		for (int j=0; j<numNodes; j++) {

			std::cout << adjacencyMatrix.at(i).at(j) << " ";
		}
		std::cout << std::endl;
	}

}


/** Auto generated Getters and setters **/
const std::vector<std::vector<int> >& Graph::getAdjacencyMatrix() const {
	return adjacencyMatrix;
}

void Graph::setAdjacencyMatrix(
		const std::vector<std::vector<int> >& adjacencyMatrix) {
	this->adjacencyMatrix = adjacencyMatrix;
}

const std::vector<Node*>& Graph::getDemandNodes() const {
	return demandNodes;
}

int Graph::getGlobalTimeLimit() const {
	return globalTimeLimit;
}

void Graph::setGlobalTimeLimit(int globalTimeLimit) {
	this->globalTimeLimit = globalTimeLimit;
}

const std::vector<Node*>& Graph::getNodes() const {
	return nodes;
}

int Graph::getNumberOfVehicles() const {
	return numberOfVehicles;
}

void Graph::setNumberOfVehicles(int numberOfVehicles) {
	this->numberOfVehicles = numberOfVehicles;
}

const std::vector<Node*>& Graph::getSupplyNodes() const {
	return supplyNodes;
}

const Node* Graph::getZeroNode() const {
	return zeroNode;
}


} /* namespace tcbvrp */
