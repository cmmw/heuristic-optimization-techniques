/*
 * LNS.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: ixi
 */

#include <cmath>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <climits>
#include "LNS.h"
#include "Constants.h"
#include "Logger.h"

extern bool quit;

namespace tcbvrp
{

LNS::LNS(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph), bestSolution(*solution), bestCosts(INT_MAX), foundBetter(false)
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
	std::vector<std::pair<Node*, Node*> > pairs;
	VariableOrder varOrder(graph.getAdjacencyMatrix());

	// check if initial solution is feasible and set bestCosts if so
	// only the last one has to be checked, the others will be correct anyway right
	if (Algorithm::calcTourCosts(*(solution->getTours().end() - 1), graph.getAdjacencyMatrix()) <= graph.getGlobalTimeLimit())
	{
		bestCosts = solution->getTotalCosts();
	}

	LOG << "";
	LOG << "removes " << removes << ":";
	LOG_NOEND << "Solution: ";
	while (removes <= REMOVE_LIMIT && removes <= (int) graph.getDemandNodes().size() && !quit)
	{
		// Choose Pairs to remove
		pairs = removeVisits(removes);

		// Variable ordering
		std::sort(pairs.begin(), pairs.end(), varOrder);

		// Reinsert the pairs
		reinsertPairs(pairs, solution->getTotalCosts());

		bestSolution.cleanEmptyTours();

		*solution = bestSolution;
		trials++;
		if (foundBetter)
		{
			foundBetter = false;
			trials = 0;

			if (removes != START_REMOVES)
			{
				removes = START_REMOVES;
				LOG << "";
				LOG << "removes " << removes << ":";
				LOG_NOEND << "Solution: ";
			}
		}

		if (trials == TRIALS_PER_COUNT)
		{
			removes++;
			LOG << "";
			if (removes <= REMOVE_LIMIT)
			{
				LOG << "removes " << removes << ":";
				LOG_NOEND << "Solution: ";
			}
			trials = 0;
		}
	}

	//No feasible solution found
	if (bestCosts == INT_MAX)
		solution->clear();
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
		tourIdx = Algorithm::random(solution->getNumberOfTours() - 1);
	} while (tours[tourIdx].size() == 0);
	nodeIdx = Algorithm::random(tours[tourIdx].size() - 2);		//-2 because we only address the first node of the pairs
	removed.push_back(std::pair<Node*, Node*>(tours[tourIdx][nodeIdx], tours[tourIdx][nodeIdx + 1]));
	removeAtPosition(std::pair<int, int>(tourIdx, nodeIdx));

	while (removed.size() < count)
	{
		nodeIdx = Algorithm::random(removed.size() - 1);
		std::vector<std::pair<std::pair<int, int>, double> > lst = rankUsingRelatedness(removed[nodeIdx], tourIdx);
		if (lst.size() == 0)
			break;
		double rnd1 = ((double) (rand()) / (RAND_MAX));
		int lstIdx = (lst.size() - 1) * (double) pow(rnd1, D);
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
			double f = relatedness(tourIndex, std::pair<Node*, Node*>(tour[nodeIdx], tour[nodeIdx + 1]), tourIdx);
			ret.push_back(std::pair<std::pair<int, int>, int>(std::pair<int, int>(tourIdx, nodeIdx), f));
		}
	}
	std::sort(ret.begin(), ret.end(), relatednessSort);
	return ret;
}

double LNS::relatedness(int tourIdx1, const std::pair<Node*, Node*>& n2, int tourIdx2) const
		{
	double c = graph.getAdjacencyMatrix()[n2.first->getId()][n2.second->getId()] / (double) graph.getMaxCosts();

	if (tourIdx1 != tourIdx2)
	{
		c += 1;
	}
	return 1 / c;
}

bool LNS::relatednessSort(const std::pair<std::pair<int, int>, int>& n1, const std::pair<std::pair<int, int>, int>& n2)
{
	return n1.second > n2.second;
}

void LNS::reinsertPairs(std::vector<std::pair<Node*, Node*> > pairs, int curCosts)
{
	if (pairs.size() == 0)
	{
		if (curCosts < bestCosts)
		{
			foundBetter = true;
			bestSolution = *solution;
			bestCosts = curCosts;
			LOG_NOEND << bestCosts << ", ";
			std::cout.flush();
		}
	} else
	{
		std::pair<Node*, Node*> currentPair = pairs.back();
		pairs.pop_back();

		std::vector<std::pair<int, int> > positions = getPositionsForPair(currentPair);

		//Value ordering
//		LNS::ValueOrder valOrder(solution->getTours(), graph.getAdjacencyMatrix());
//		std::sort(positions.begin(), positions.end(), valOrder);

		for (std::vector<std::pair<int, int> >::iterator it = positions.begin(); it != positions.end() && !quit; it++)
		{
			bool skipSubtree = false;
			int delta;

			// -1 marks the special position to create a new empty tour
			if (it->first == -1)
			{
				std::vector<Node*> tour;
				solution->addTour(tour);

				it->first = solution->getNumberOfTours() - 1;
			}

			delta = insertAtPosition(currentPair, *it);

			if ((delta + curCosts + (graph.getMinCosts() * (int) pairs.size() * 3) - (graph.getMaxCosts() * (int) pairs.size())) >= bestCosts)		//cheap lower bound heuristic
			{
				skipSubtree = true;
			} else
			{
				int cfree = 0;
				for (std::vector<std::vector<Node*> >::iterator tour = solution->getTours().begin(); tour != solution->getTours().end(); tour++)
				{
					int ccur = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
					if (ccur > (int) graph.getGlobalTimeLimit())
					{
						skipSubtree = true;
						break;
					}
					cfree += (graph.getGlobalTimeLimit() - ccur);
				}

				if (!skipSubtree)
				{
					cfree += (graph.getNumberOfVehicles() - solution->getNumberOfTours()) * graph.getGlobalTimeLimit();
					if (cfree < (int) pairs.size() * ((3 * graph.getMinCosts()) - graph.getMaxCosts()))
					{
						skipSubtree = true;
					}
				}
			}
			if (!skipSubtree)
				reinsertPairs(pairs, curCosts + delta);

			if (foundBetter)
				return;

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

std::vector<std::pair<int, int> > LNS::getPositionsForPair(std::pair<Node*, Node*> pair)
{
	std::vector<std::pair<int, int> > pairs;

	bool type; // type of pair: true = S->D, false = D->S
	bool emptyTourExists = false;
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
		if (solution->getTours().at(tour_number).size() == 0)
		{
			emptyTourExists = true;
		}
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

	// if type S->D add a special position -1, 0 to generate new tour if there an empty tour doesn't exist already
	if (type && !emptyTourExists && ((int) solution->getTours().size()) < graph.getNumberOfVehicles())
	{
		pairs.push_back(std::make_pair(-1, 0));
	}

	return pairs;
}

bool LNS::VariableOrder::operator()(const std::pair<Node*, Node*>& p1, const std::pair<Node*, Node*>& p2)
{
	if (matrix[p1.first->getId()][p1.second->getId()] < matrix[p2.first->getId()][p2.second->getId()])
		return true;
	return false;
}

bool LNS::ValueOrder::operator()(const std::pair<int, int>& p1, const std::pair<int, int>& p2)
{
	Node zeroNode(Node::ZERO, 0);

	const std::vector<Node*>& tour1 = tours[p1.first];
	const std::vector<Node*>& tour2 = tours[p2.first];
	Node* n11;
	Node* n12;
	Node* n21;
	Node* n22;

	if (p1.second == 0)
		n11 = &zeroNode;
	else
		n11 = tour1[p1.second - 1];

	if (p1.second == (int) tour1.size())
		n12 = &zeroNode;
	else
		n12 = tour1[p1.second];

	if (p2.second == 0)
		n21 = &zeroNode;
	else
		n21 = tour2[p2.second - 1];

	if (p2.second == (int) tour2.size())
		n22 = &zeroNode;
	else
		n22 = tour2[p2.second];

	if (matrix[n11->getId()][n12->getId()] > matrix[n21->getId()][n22->getId()])
		return true;
	return false;
}

}
/* namespace tcbvrp */
