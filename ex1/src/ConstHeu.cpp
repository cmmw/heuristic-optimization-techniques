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
#include <list>

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

	/***/
	//Print list of neighbours with costs
	for (int i = 0; i < graph.getNumberOfNodes(); i++)
	{
		std::cout << "Best neighbors for node " << i << std::endl;
		for (std::vector<Node*>::iterator it = bestNeighbors[i].begin(); it != bestNeighbors[i].end(); it++)
		{
			std::cout << (*it)->getId() << ", ";
		}
		std::cout << std::endl;

		std::cout << "Costs for neighbor of node " << i << std::endl;
		for (std::vector<Node*>::iterator it = bestNeighbors[i].begin(); it != bestNeighbors[i].end(); it++)
		{
			std::cout << graph.getAdjacencyMatrix()[i][(*it)->getId()] << ", ";
		}
		std::cout << std::endl;
		std::cout << "--------------" << std::endl;
	}
	/***/

	/*Tour construction starts here*/
	const std::vector<std::vector<int> > &matrix = graph.getAdjacencyMatrix();
	std::vector<std::vector<Node*> > tours;
	for (std::vector<Node*>::const_iterator it = supplyNodes.begin(); it != supplyNodes.end(); it++)
	{
		Node* tmpNode = (*it);
		if (tmpNode->getVisited())			//if this supply node is already visited get the next one
			continue;
		std::vector<Node*> tour;
		tour.push_back(tmpNode);
		tmpNode->setVisited(true);
		int curCosts = graph.getAdjacencyMatrix()[0][tmpNode->getId()];		//cost from 0 to tmpNode
		while (true)
		{

			//take cheapest unvisited node within the neighbors of tmpNode
			Node* cheapest = NULL;
			for (std::vector<Node*>::iterator iit = bestNeighbors[tmpNode->getId()].begin(); iit != bestNeighbors[tmpNode->getId()].end(); iit++)
			{
				if (!(*iit)->getVisited())
				{
					cheapest = (*iit);
				}
			}

			//There are no unvisited nodes left
			if (cheapest == NULL)
				break;

			Node* prvNode = tmpNode;
			tmpNode = cheapest;

			//check if time to visit tmpNode + time to visit 0 node is below the time limit
			if (curCosts + matrix[prvNode->getId()][tmpNode->getId()] + matrix[tmpNode->getId()][0] <= graph.getGlobalTimeLimit())
			{
				curCosts += matrix[prvNode->getId()][tmpNode->getId()];
				tmpNode->setVisited(true);
				tour.push_back(tmpNode);
			} else		//if not, close this tour
			{
				break;
			}
		}

		//Don't add tours like 0 -> S -> 0 (can happen at the very last iteration, if there are only supply nodes left but no demand node)
		if (tour.size() > 1)
		{
			//Before adding a tour, check if the last node is a supply node, if so remove it, set it to unvisited and
			//set the iterator to the beginning so we don't miss this supply node
			if (tour.back()->getType() == Node::SUPPLY)
			{
				tour.back()->setVisited(false);
				tour.pop_back();
				it = supplyNodes.begin();
			}
			tours.push_back(tour);
		}
	}

	/*Print results*/
	std::cout << "tours: " << std::endl;
	int c = 0;
	int totalCosts = 0;
	for (std::vector<std::vector<Node*> >::iterator tour = tours.begin(); tour != tours.end(); tour++)
	{
		std::cout << std::endl;
		c++;
		std::cout << "tour " << c << ":" << std::endl;
		for (std::vector<Node*>::iterator it = tour->begin(); it != tour->end(); it++)
		{
			std::string type = ((*it)->getType() == Node::SUPPLY) ? "S" : "D";
			std::cout << (*it)->getId() << type << ", ";
		}
		std::cout << std::endl;

		std::cout << "costs: " << std::endl;
		int costs = 0;
		costs += matrix[0][(*tour->begin())->getId()];
		for (std::vector<Node*>::iterator it1 = tour->begin(), it2 = it1 + 1; it2 != tour->end(); it1++, it2++)
		{
			costs += matrix[(*it1)->getId()][(*it2)->getId()];
			if ((it2 + 1) == tour->end())
			{
				costs += matrix[(*it2)->getId()][0];
			}
		}
		std::cout << costs << std::endl;
		totalCosts += costs;
	}
	std::cout << std::endl << tours.size() << " tours created, allowed: " << graph.getNumberOfVehicles() << std::endl;
	std::cout << "Total costs: " << totalCosts << std::endl;

}

} /* namespace tcbvrp */
