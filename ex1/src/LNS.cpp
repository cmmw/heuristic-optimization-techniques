/*
 * LNS.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: ixi
 */

#include "LNS.h"

#include "Constants.h"

namespace tcbvrp
{

LNS::LNS(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph)
{

}

LNS::~LNS()
{
}

void LNS::solve()
{
	// the overall search procedure
	int removes = START_REMOVES;
	int trials = 0;
	std::vector<std::pair<Node*, Node*> > visits;

	while (removes <= REMOVE_LIMIT)
	{
		if (trials == TRIALS_PER_COUNT)
		{
			removes++;
			trials = 0;
		}

		// Save old solution
		Solution old = *solution;

		// Choose Visits to remove
		visits = removeVisits(removes);

		// Reinsert the visits
		reinsertVisits(visits);

		if (old.getTotalCosts() < solution->getTotalCosts())
		{
			*solution = old;
		}
	}
}

std::vector<std::pair<Node*, Node*> > LNS::removeVisits(int count)
{
	std::vector<std::pair<Node*, Node*> > temp;
	return temp;
}

void LNS::reinsertVisits(std::vector<std::pair<Node*, Node*> > nodeIds)
{
}

} /* namespace tcbvrp */
