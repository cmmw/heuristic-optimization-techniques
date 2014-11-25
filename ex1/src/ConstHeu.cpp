/*
 * ConsHeu.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#include "ConstHeu.h"
#include "NodeCompare.h"
#include <algorithm>
#include <iostream>

namespace tcbvrp
{

ConstHeu::ConstHeu(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph)
{
	// TODO Auto-generated constructor stub

}

ConstHeu::~ConstHeu()
{
	// TODO Auto-generated destructor stub
}

void ConstHeu::solve()
{
	const std::vector<Node*> &supplyNodes = graph.getSupplyNodes();
	const std::vector<Node*> &demandNodes = graph.getDemandNodes();

	std::vector<std::vector<Node*> > bestNeighbors;		//list of best neighbors (supply-nodes for demand-nodes and vice versa)

	bestNeighbors.resize(graph.getNumberOfNodes());

	//Fill in sorted neighbors for demand-nodes in bestNeighbors
	for (unsigned int i = 0; i < demandNodes.size(); i++)
	{
		NodeCompare comp(graph.getAdjacencyMatrix()[demandNodes[i]->getId()]);
		std::vector<Node*> nodes = supplyNodes;
		std::sort(nodes.begin(), nodes.end(), comp);
		bestNeighbors[demandNodes[i]->getId()] = nodes;
	}

	//Fill in sorted neighbors for supply-nodes in bestNeighbors
	for (unsigned int i = 0; i < supplyNodes.size(); i++)
	{
		NodeCompare comp(graph.getAdjacencyMatrix()[supplyNodes[i]->getId()]);
		std::vector<Node*> nodes = demandNodes;
		std::sort(nodes.begin(), nodes.end(), comp);
		bestNeighbors[supplyNodes[i]->getId()] = nodes;
	}

	for (int i = 0; i < graph.getNumberOfNodes(); i++)
	{
		std::cout << "Best neighbors for node " << i << std::endl;
		for (std::vector<Node*>::iterator it = bestNeighbors[i].begin(); it != bestNeighbors[i].end(); it++)
		{
			std::cout << (*it)->getId() << ", ";
		}
		std::cout << std::endl;

		std::cout << "Weights for neighbor of node " << i << std::endl;
		for (std::vector<Node*>::iterator it = bestNeighbors[i].begin(); it != bestNeighbors[i].end(); it++)
		{
			std::cout << graph.getAdjacencyMatrix()[i][(*it)->getId()] << ", ";
		}
		std::cout << std::endl;
		std::cout << "--------------" << std::endl;

	}

}

} /* namespace tcbvrp */
