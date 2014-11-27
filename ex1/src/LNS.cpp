/*
 * LNS.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: ixi
 */

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "LNS.h"
#include "Constants.h"

namespace tcbvrp
{

LNS::LNS(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph), bestSolution(*solution), bestCosts(solution->getTotalCosts())
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

	while (removes <= REMOVE_LIMIT && removes <= (graph.getNumberOfNodes() - 1) / 2)
	{
		if (trials == TRIALS_PER_COUNT)
		{
			removes++;
			trials = 0;
		}

		// Choose Pairs to remove
		pairs = removeVisits(removes);

		// Reinsert the pairs
		reinsertPairs(pairs, solution->getTotalCosts());

		*solution = bestSolution;
		bestCosts = solution->getTotalCosts();
		trials++;
	}
}

std::vector<std::pair<Node*, Node*> > LNS::removeVisits(unsigned int count)
{
	std::vector<std::pair<Node*, Node*> > removed;
	std::vector<int> tourIndex;
	std::vector<std::vector<Node*> >& tours = solution->getTours();
	unsigned int tourIdx;
	unsigned int nodeIdx;
	/*TODO this should be done more efficient*/
	do
	{
		tourIdx = random(solution->getNumberOfTours());
	} while (tours[tourIdx].size() == 0);
	nodeIdx = random(tours[tourIdx].size() - 1);
	removed.push_back(std::pair<Node*, Node*>(tours[tourIdx][nodeIdx], tours[tourIdx][nodeIdx + 1]));
	tourIndex.push_back(tourIdx);
	removeAtPosition(std::pair<int, int>(tourIdx, nodeIdx));

	while (removed.size() < count)
	{
		nodeIdx = random(removed.size());
		std::vector<std::pair<std::pair<int, int>, double> > lst = rankUsingRelatedness(removed[nodeIdx], tourIndex[nodeIdx]);
		if (lst.size() == 0)
			break;
		double rnd1 = (double) rand() - 1;
		rnd1 = (rnd1 == -1) ? 0 : rnd1;
		double rnd2 = (rnd1 / (RAND_MAX));
		int lstIdx = (lst.size() - 1) * pow(rnd2, D);

		tourIdx = lst[lstIdx].first.first;
		nodeIdx = lst[lstIdx].first.second;

		std::vector<Node*> &row = solution->getTours()[tourIdx];

		Node* n1 = row[nodeIdx];
		Node* n2 = row[nodeIdx + 1];
		removed.push_back(std::pair<Node*, Node*>(n1, n2));
		removeAtPosition(std::pair<int, int>(tourIdx, nodeIdx));
	}
	return removed;
}

std::vector<std::pair<std::pair<int, int>, double> > LNS::rankUsingRelatedness(const std::pair<Node*, Node*>& nodePair, int tourIndex) const
		{
	std::vector<std::pair<std::pair<int, int>, double> > ret;
	for (unsigned int tourIdx = 0; tourIdx < solution->getTours().size(); tourIdx++)
	{
		std::vector<Node*> &tour = solution->getTours()[tourIdx];
		for (unsigned int nodeIdx = 0; nodeIdx + 1 < tour.size(); nodeIdx++)
		{
			double f = relatedness(nodePair, tourIndex, std::pair<Node*, Node*>(tour[nodeIdx], tour[nodeIdx + 1]), tourIdx);
			ret.push_back(std::pair<std::pair<int, int>, int>(std::pair<int, int>(tourIdx, nodeIdx), f));
		}
	}
	std::sort(ret.begin(), ret.end(), sortNodes);
	return ret;
}

double LNS::relatedness(const std::pair<Node*, Node*>& n1, int tourIdx1, const std::pair<Node*, Node*>& n2, int tourIdx2) const
		{
	double c = graph.getAdjacencyMatrix()[n2.first->getId()][n2.second->getId()] / (double) graph.getMaxCosts();
	if (tourIdx1 == tourIdx2)
	{
		c += 1;
	}
	return 1 / c;
}

bool LNS::sortNodes(const std::pair<std::pair<int, int>, int>& n1, const std::pair<std::pair<int, int>, int>& n2)
{
	return n1.first < n2.first;
}

void LNS::reinsertPairs(std::vector<std::pair<Node*, Node*> > pairs, int curCosts)
{
	if (pairs.size() == 0)
	{
		std::cout << "***" << std::endl;
		std::cout << curCosts << ":" << bestCosts << std::endl;
		std::cout << solution->getTotalCosts() << ":" << bestSolution.getTotalCosts() << std::endl;
		std::cout << "---" << std::endl;
		if (curCosts < bestCosts)
		{
			bestSolution = *solution;
			bestSolution.printSolution();
		}
	} else
	{
		std::pair<Node*, Node*> currentPair = pairs.back();
		pairs.pop_back();

		std::vector<std::pair<int, int> > positions = getPositionsForPair(currentPair);
		for (std::vector<std::pair<int, int> >::iterator it = positions.begin(); it != positions.end(); it++)
		{
			bool violated = false;
			int delta;
			delta = insertAtPosition(currentPair, *it);
			for (std::vector<std::vector<Node*> >::iterator tour = solution->getTours().begin(); tour != solution->getTours().end(); tour++)
			{
				if (Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix()) > graph.getGlobalTimeLimit())
				{
					violated = true;
					break;
				}
			}
			if (!violated)
				reinsertPairs(pairs, curCosts + delta);
			removeAtPosition(*it);
		}
	}
}

int LNS::insertAtPosition(std::pair<Node*, Node*> pair, std::pair<int, int> position)
{
	const std::vector<std::vector<int> >& matrix = graph.getAdjacencyMatrix();
	int delta;
	std::vector<Node*> &tour = solution->getTours()[position.first];

	const Node *n1, *n2;
	if (position.second == 0)
		n1 = graph.getZeroNode();
	else
		n1 = tour[position.second - 1];

	if (position.second == (int) tour.size())
		n2 = graph.getZeroNode();
	else
		n2 = tour[position.second];

	delta = matrix[pair.first->getId()][pair.second->getId()];
	delta -= matrix[n1->getId()][n2->getId()];
	delta += matrix[n1->getId()][pair.first->getId()];
	delta += matrix[pair.second->getId()][n2->getId()];
	tour.insert(tour.begin() + position.second, pair.second);
	tour.insert(tour.begin() + position.second, pair.first);
	return delta;
}

void LNS::removeAtPosition(std::pair<int, int> position)
{
	std::vector<Node*>& tour = solution->getTours()[position.first];
	tour.erase(tour.begin() + position.second, tour.begin() + position.second + 2);
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
