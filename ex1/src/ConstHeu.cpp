/*
 * ConsHeu.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#include <iostream>
#include <list>
#include "ConstHeu.h"


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

	solution->clear();
	bestNeighbors.resize(graph.getNumberOfNodes());

	//Sort neighbors for demand-nodes in bestNeighbors
	for (unsigned int i = 0; i < demandNodes.size(); i++)
	{
		bestNeighbors[demandNodes[i]->getId()] = supplyNodes;
		Algorithm::sortNeighborsByAscEdgeCosts(bestNeighbors[demandNodes[i]->getId()], graph.getAdjacencyMatrix()[demandNodes[i]->getId()]);
	}

	//Sort neighbors for supply-nodes in bestNeighbors
	for (unsigned int i = 0; i < supplyNodes.size(); i++)
	{
		bestNeighbors[supplyNodes[i]->getId()] = demandNodes;
		Algorithm::sortNeighborsByAscEdgeCosts(bestNeighbors[supplyNodes[i]->getId()], graph.getAdjacencyMatrix()[supplyNodes[i]->getId()]);
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
	for (std::vector<Node*>::const_iterator it = supplyNodes.begin(); it != supplyNodes.end(); it++)
	{
		Node* tmpNode = (*it);
		if (tmpNode->getVisited())			//if this supply node is already visited go to the next one
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
			//Before adding a tour, check if the last node is a supply node, if so remove it, set it to unvisited
			if (tour.back()->getType() == Node::SUPPLY)
			{
				tour.back()->setVisited(false);
				tour.pop_back();
			}
			solution->addTour(tour);
		}
	}

	/*Print results*/
	solution->printSolution();
	std::cout << solution->getNumberOfTours() << " tours created, allowed: " << graph.getNumberOfVehicles() << std::endl;
}

} /* namespace tcbvrp */
