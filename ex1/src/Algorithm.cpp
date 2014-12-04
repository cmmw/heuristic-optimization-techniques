/*
 * Algorithm.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: christian
 */

#include <algorithm>
#include "Algorithm.h"
#include "NodeCompare.h"

namespace tcbvrp
{

Algorithm::Algorithm(Solution* solution, const Graph& graph) :
		solution(solution), graph(graph)
{
}

Algorithm::~Algorithm()
{
}

unsigned int Algorithm::calcTourCosts(const std::vector<Node*> &tour, const std::vector<std::vector<int> >& matrix)
{
	int costs = 0;
	if (tour.empty())
		return costs;
	costs += matrix[0][(*tour.begin())->getId()];
	for (std::vector<Node*>::const_iterator it1 = tour.begin(), it2 = it1 + 1; it2 != tour.end(); it1++, it2++)
	{
		costs += matrix[(*it1)->getId()][(*it2)->getId()];
		if ((it2 + 1) == tour.end())
		{
			costs += matrix[(*it2)->getId()][0];
		}
	}
	return costs;
}

void Algorithm::sortNeighborsByAscEdgeCosts(std::vector<Node*> &nodes, const std::vector<int>& costRow)
{
	NodeCompare comp(costRow);
	std::sort(nodes.begin(), nodes.end(), comp);
}

unsigned int Algorithm::random(unsigned int max)
{
	return rand() % (max + 1);
}

} /* namespace tcbvrp */
