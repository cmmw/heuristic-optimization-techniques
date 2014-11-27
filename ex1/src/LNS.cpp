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
	std::vector<std::pair<Node*, Node*> > pairs;

	while (removes <= REMOVE_LIMIT)
	{
		if (trials == TRIALS_PER_COUNT)
		{
			removes++;
			trials = 0;
		}

		// Save old solution
		Solution old = *solution;

		// Choose Pairs to remove
		pairs = removeVisits(removes);

		// Reinsert the pairs
		reinsertPairs(pairs);

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
	unsigned int r1;

	r1 = random(tours[row].size() - 1);

	removed.push_back(std::pair<Node*, Node*>(tours[row][r1], tours[row][r1 + 1]));
	removeAtPosition(std::pair<int, int>(row, r1));

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
		removed.push_back(std::pair<Node*, Node*>(n1, n2));
		removeAtPosition(std::pair<int, int>(rowIdx, elIdx));
	}
	return removed;
}

void LNS::reinsertPairs(std::vector<std::pair<Node*, Node*> > pairs)
{
	if (pairs.size() == 0)
	{
		if (solution->getTotalCosts() < bestSolution.getTotalCosts())
		{
			bestSolution = *solution;
		}
	}
	else
	{

		std::pair<Node*, Node*> currentPair = pairs.back();
		pairs.pop_back();

		std::vector<std::pair<int, int> > positions = getPositionsForPair(currentPair);

		for (std::vector<std::pair<int, int> >::iterator it = positions.begin(); it != positions.end(); it++)
		{
			insertAtPosition(currentPair, *it);
			reinsertPairs(pairs);
			removeAtPosition(*it);
		}
	}
}

void LNS::insertAtPosition(std::pair<Node*, Node*> pair, std::pair<int, int> position)
{
	solution->getTours()[position.first].insert(solution->getTours()[position.first].begin() + position.second, pair.second);
	solution->getTours()[position.first].insert(solution->getTours()[position.first].begin() + position.second, pair.first);
}

void LNS::removeAtPosition(std::pair<int, int> position)
{
	solution->getTours()[position.first].erase(solution->getTours()[position.first].begin() + position.second, solution->getTours()[position.first].begin() + position.second + 2);
}

unsigned int LNS::random(unsigned int max)
{
	return rand() % max;
}

std::vector<std::pair<int, int> > LNS::getPositionsForPair(std::pair<Node*, Node*> pair)
{
	std::vector<std::pair<int, int> > pairs;

	bool type; // type of pair: true = S->D, false = D->S
	if (pair.first->getType() == Node::SUPPLY)
	{
		type = true;
	}
	else
	{
		type = false;
	}

	for (unsigned tour_number = 0; tour_number < solution->getTours().size(); tour_number++)
	{
		int offset = 0; // if the type is S->D all even positions including 0 will be considered
		if (!type)
		{
			offset = 1; // in case of D->S all odd positions will be considered
		}
		for (unsigned position = 0 + offset; position <= solution->getTours().at(tour_number).size(); position += 2)
		{
			pairs.push_back(std::make_pair(tour_number, position));
		}
	}

	return pairs;
}

} /* namespace tcbvrp */
