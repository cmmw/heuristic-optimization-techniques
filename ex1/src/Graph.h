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

	static Graph createGraph(std::string filename);

	int getNumberOfNodes();

	void printGraph();

	/** Auto generated Getters and setters **/
	const std::vector<std::vector<int> >& getAdjacencyMatrix() const;

	void setAdjacencyMatrix(const std::vector<std::vector<int> >& adjacencyMatrix);

	const std::vector<Node*>& getDemandNodes() const;

	int getGlobalTimeLimit() const;

	void setGlobalTimeLimit(int globalTimeLimit);

	const std::vector<Node*>& getNodes() const;

	int getNumberOfVehicles() const;

	void setNumberOfVehicles(int numberOfVehicles);

	const std::vector<Node*>& getSupplyNodes() const;

	const Node* getZeroNode() const;

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
