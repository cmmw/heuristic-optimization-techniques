/*
 * Graph.h
 *
 *  Created on: Nov 21, 2014
 *      Author: ixi
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>
#include <vector>
#include "Node.h"

namespace tcbvrp {

class Graph {
public:
	Graph();
	virtual ~Graph();

	void addNode(Node* node);
	void setAdjacencyMatrix(std::vector< std::vector<int> > adjacencyMatrix);

	static Graph createGraph(std::string filename);

	int getNumberOfNodes();

private:
	int globalTimeLimit;
	int numberOfVehicles;

	std::vector<Node*> nodes;
	std::vector<Node*> supplyNodes;
	std::vector<Node*> demandNodes;
	Node *zeroNode;

	std::vector< std::vector<int> > adjacencyMatrix;
};

} /* namespace tcbvrp */

#endif /* GRAPH_H_ */
