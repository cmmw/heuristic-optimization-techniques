/*
 * ConsHeu.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#include <iostream>
#include <list>
#include <cstdlib>
#include <climits>

#include "RandConstHeu.h"
#include "Constants.h"

namespace tcbvrp
{

RandConstHeu::RandConstHeu(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph)
{
}

RandConstHeu::~RandConstHeu()
{
}

void RandConstHeu::solve()
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
			//CL = list of unvisited nodes
			std::vector<Node*> CL;
			int cmin = INT_MAX, cmax = 0;
			for (std::vector<Node*>::iterator iit = bestNeighbors[tmpNode->getId()].begin(); iit != bestNeighbors[tmpNode->getId()].end(); iit++)
			{
				if (!(*iit)->getVisited())
				{
					CL.push_back((*iit));
					int c = graph.getAdjacencyMatrix()[tmpNode->getId()][(*iit)->getId()];
					if (c < cmin)
						cmin = c;
					if (c > cmax)
						cmax = c;
				}
			}

			//There are no unvisited nodes left
			if (CL.empty())
				break;

			Node* prvNode = tmpNode;

			//create RCL from CL
			std::vector<Node*> RCL;
			for (std::vector<Node*>::iterator n = CL.begin(); n != CL.end(); n++)
			{
				if (graph.getAdjacencyMatrix()[tmpNode->getId()][(*n)->getId()] <= cmin + ALPHA * (cmax - cmin))
					RCL.push_back(*n);
			}

			//random selection from RCL
			tmpNode = RCL[Algorithm::random(RCL.size() - 1)];

			//check if time to visit tmpNode + time to visit 0 node is below the time limit
			if (curCosts + matrix[prvNode->getId()][tmpNode->getId()] + matrix[tmpNode->getId()][0] <= graph.getGlobalTimeLimit() || solution->getNumberOfTours() == graph.getNumberOfVehicles() - 1)
			{
				curCosts += matrix[prvNode->getId()][tmpNode->getId()];
				tmpNode->setVisited(true);
				tour.push_back(tmpNode);
			} else //if not, close this tour (only if there are vehicles left, otherwise violate the time limit in the last tour)
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
}

} /* namespace tcbvrp */
