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
}

ConstHeu::~ConstHeu()
{
}

void ConstHeu::solve()
{
	const std::vector<Node*> &supplyNodes = graph.getSupplyNodes();
	const std::vector<Node*> &demandNodes = graph.getDemandNodes();

	std::vector<std::vector<Node*> > bestNeighbors;		//list of best neighbors (supply-nodes for demand-nodes and vice versa)

	solution->clear();
	graph.unvisitNodes();
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

	//Sort neighbors (supply nodes) for node 0
	bestNeighbors[0] = supplyNodes;
	Algorithm::sortNeighborsByAscEdgeCosts(bestNeighbors[0], graph.getAdjacencyMatrix()[0]);

	/*Tour construction starts here*/
	const std::vector<std::vector<int> > &matrix = graph.getAdjacencyMatrix();
	for (std::vector<Node*>::const_iterator it = bestNeighbors[0].begin(); it != bestNeighbors[0].end(); it++)
	{
		Node* tmpNode = (*it);
		if (tmpNode->getVisited())			//if this supply node is already visited go to the next one
			continue;
		std::vector<Node*> tour;
		tour.push_back(tmpNode);
		tmpNode->setVisited(true);
		unsigned int curCosts = graph.getAdjacencyMatrix()[0][tmpNode->getId()];		//cost from 0 to tmpNode
		while (true)
		{
			//take cheapest unvisited node within the neighbors of tmpNode
			Node* cheapest = NULL;
			for (std::vector<Node*>::iterator iit = bestNeighbors[tmpNode->getId()].begin(); iit != bestNeighbors[tmpNode->getId()].end(); iit++)
			{
				if (!(*iit)->getVisited())
				{
					cheapest = (*iit);
					break;
				}
			}

			//There are no unvisited nodes left
			if (cheapest == NULL)
				break;

			Node* prvNode = tmpNode;
			tmpNode = cheapest;

			//check if time to visit tmpNode + time to visit 0 node is below the time limit
			if (curCosts + matrix[prvNode->getId()][tmpNode->getId()] + matrix[tmpNode->getId()][0] <= graph.getGlobalTimeLimit() || solution->getNumberOfTours() == graph.getNumberOfVehicles() - 1)
			{
				curCosts += matrix[prvNode->getId()][tmpNode->getId()];
				tmpNode->setVisited(true);
				tour.push_back(tmpNode);
			} else //if not, close this tour (only if there are vehicles left, otherwise violate the time limit in the last tour)
			{
				//check if current cost + cost to reach node 0 exceeds the global time limit, remove last node until tour is feasible
				Node* last = tour.back();
				int ec = matrix[last->getId()][0];
				while (curCosts + ec > graph.getGlobalTimeLimit())
				{
					last->setVisited(false);
					tour.pop_back();
					curCosts -= matrix[tour.back()->getId()][last->getId()];
					last = tour.back();
					ec = matrix[last->getId()][0];
				}
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
}

} /* namespace tcbvrp */
