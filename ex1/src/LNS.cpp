/*
 * LNS.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: ixi
 */

#include <cstdlib>
#include <cmath>

#include "LNS.h"
#include "Constants.h"

namespace tcbvrp
{

LNS::LNS(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph)
{
	srand(time(NULL));
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

std::vector<std::pair<Node*, Node*> > LNS::removeVisits(unsigned int count)
{
	std::vector<std::pair<Node*, Node*> > removed(count);
	std::vector<std::vector<Node*> >& tours = solution->getTours();
	unsigned int row = random(solution->getNumberOfTours());
	unsigned int r1, r2;

	r1 = random(tours[row].size() - 1);
	r2 = r1 + 1;

	removed.push_back(std::pair<Node*, Node*>(tours[row][r1], tours[row][r2]));
	tours[row].erase(tours[row].begin() + r1);
	tours[row].erase(tours[row].begin() + r2);

	while (removed.size() < count)
	{
		r1 = random(removed.size());

		std::vector<std::pair<int, int> > lst; // = rankUsingRelatedness(removed[r1]);		//c_ij = cost of pair<n1,n2> = cost n1 -> n2

		double rnd1 = (double) rand() - 1;
		rnd1 = (rnd1 == -1) ? 0 : rnd1;
		double rnd2 = (rnd1 / (RAND_MAX));
		int idx = (lst.size() - 1) * pow(rnd2, D);

		int rowIdx = lst[idx].first;
		int elIdx = lst[idx].second;

		std::vector<Node*> &row = solution->getTours()[rowIdx];

		Node* n1 = row[elIdx];
		Node* n2 = row[elIdx + 1];
		row.erase(row.begin() + elIdx);
		row.erase(row.begin() + elIdx + 1);

		removed.push_back(std::pair<Node*, Node*>(n1, n2));
	}
	return removed;
}

void LNS::reinsertVisits(std::vector<std::pair<Node*, Node*> > nodeIds)
{
}

unsigned int LNS::random(unsigned int max)
{
	return rand() % max;
}

} /* namespace tcbvrp */
