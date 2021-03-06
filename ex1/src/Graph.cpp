/*
 * Graph.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: ixi
 */

#include <climits>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Graph.h"
#include "Logger.h"

namespace tcbvrp
{

int Graph::maxCosts = 0;
int Graph::minCosts = INT_MAX;

Graph::Graph() :
		globalTimeLimit(-1), numberOfVehicles(-1), zeroNode(0)
{

}

Graph::~Graph()
{
	for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++)
	{
		delete *it;
	}
}

void Graph::addNode(Node* node)
{
	nodes.push_back(node);

	switch (node->getType())
	{
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

int Graph::getNumberOfNodes() const
{
	return nodes.size();
}

/**
 * Reads given textfile and creates Graph objects as specified in the file.
 */
Graph Graph::createGraph(std::string filename)
{

	std::ifstream input(filename.c_str());
	std::string line;
	int linenr = 0;
	std::string ref = "0123456789";
	Graph graph;
	std::vector<std::vector<int> > adjacencyMatrix;

	int numNodes = -1;
	int count = -1;
	int count2 = -1;
	int numVehicles = -1;
	int globalTimeLimit = -1;

	Node *zero = new Node(Node::ZERO, 0);
	graph.addNode(zero);

	if (!input.is_open())
	{
		std::string msg = "Could not open file " + filename;
		throw std::runtime_error(msg);
	}

	while (std::getline(input, line))
	{
		linenr++;

		if (ref.find(line[0]) == std::string::npos)
		{
			// Not a graph line (must start with a number)
			continue;
		}
		else
		{
			std::istringstream converter(line);

			if (numNodes == -1)
			{
				converter >> numNodes;
				count = numNodes;
				count2 = numNodes + 1;
			}
			else if (globalTimeLimit == -1)
			{
				converter >> globalTimeLimit;
				graph.setGlobalTimeLimit(globalTimeLimit);

			}
			else if (numVehicles == -1)
			{
				converter >> numVehicles;
				graph.setNumberOfVehicles(numVehicles);

			}
			else if (count > 0)
			{
				char type;
				int id;
				Node *node;

				converter >> id;
				converter >> type;

				switch (type)
				{
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
			else if (count == 0 && count2 > 0)
			{

				int weight;
				std::vector<int> row;

				for (int i = 0; i < numNodes + 1; i++)
				{
					converter >> weight;
					row.push_back(weight);
					if (weight > maxCosts)
						maxCosts = weight;
					if (weight != 0 && weight < minCosts)
						minCosts = weight;
				}
				adjacencyMatrix.push_back(row);

				// Count down all matrix lines that are read
				count2--;
			}

			graph.setAdjacencyMatrix(adjacencyMatrix);

			if (converter.fail())
			{
				std::cout << linenr << ": Invalid input format." << std::endl;
			}

		}
	}

	return graph;
}

void Graph::printGraph()
{
	bool printTextDescription = true;
	bool printZeroNode = true;
	int numNodes = getNumberOfNodes();

	if (printTextDescription)
	{
		LOG << "Number of nodes: ";
	}
	LOG << numNodes;

	if (printTextDescription)
	{
		LOG << "Global time limit: ";
	}
	LOG << getGlobalTimeLimit();

	if (printTextDescription)
	{
		LOG << "Number of vehicles: ";
	}
	LOG << getNumberOfVehicles();

	if (printTextDescription)
	{
		LOG << "Nodes: ";
	}

	for (int i = 0; i < numNodes; i++)
	{
		if (i == 0 && !printZeroNode)
		{
			continue;
		}
		std::string type;

		switch (nodes.at(i)->getType())
		{
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

		LOG << i << " " << type;
	}

	if (printTextDescription)
	{
		LOG << "Adjacency Matrix: ";
	}
	for (int i = 0; i < numNodes; i++)
	{
		for (int j = 0; j < numNodes; j++)
		{

			LOG_NOEND << adjacencyMatrix.at(i).at(j) << " ";
		}
		LOG << "";
	}

}

/** Auto generated Getters and setters **/
const std::vector<std::vector<int> >& Graph::getAdjacencyMatrix() const
{
	return adjacencyMatrix;
}

void Graph::setAdjacencyMatrix(
		const std::vector<std::vector<int> >& adjacencyMatrix)
{
	this->adjacencyMatrix = adjacencyMatrix;
}

const std::vector<Node*>& Graph::getDemandNodes() const
{
	return demandNodes;
}

unsigned int Graph::getGlobalTimeLimit() const
{
	return globalTimeLimit;
}

void Graph::setGlobalTimeLimit(int globalTimeLimit)
{
	this->globalTimeLimit = globalTimeLimit;
}

const std::vector<Node*>& Graph::getNodes() const
{
	return nodes;
}

int Graph::getNumberOfVehicles() const
{
	return numberOfVehicles;
}

void Graph::setNumberOfVehicles(int numberOfVehicles)
{
	this->numberOfVehicles = numberOfVehicles;
}

const std::vector<Node*>& Graph::getSupplyNodes() const
{
	return supplyNodes;
}

Node* Graph::getZeroNode() const
{
	return zeroNode;
}

int Graph::getMaxCosts() const
{
	return this->maxCosts;
}

int Graph::getMinCosts() const
{
	return this->minCosts;
}

void Graph::unvisitNodes() const
{
	for (std::vector<Node*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
		(*it)->setVisited(false);
	zeroNode->setVisited(false);
}

} /* namespace tcbvrp */
