/*
 * LNS.h
 *
 *  Created on: Nov 27, 2014
 *      Author: ixi
 */

#ifndef LNS_H_
#define LNS_H_

#include <vector>
#include <utility>
#include "Algorithm.h"

namespace tcbvrp
{

class LNS: public Algorithm
{
public:
	LNS(Solution* solution, const Graph& graph);
	virtual ~LNS();

	void solve();

private:

	class VariableOrder
	{
	public:
		VariableOrder(const std::vector<std::vector<int> >& matrix) :
				matrix(matrix)
		{
		}

		bool operator()(const std::pair<Node*, Node*>& p1, const std::pair<Node*, Node*>& p2);

	private:
		std::vector<std::vector<int> > matrix;
	};

	class ValueOrder
	{
	public:
		ValueOrder(const std::vector<std::vector<Node*> >& tours, const std::vector<std::vector<int> >& matrix) :
				tours(tours), matrix(matrix)
		{
		}

		bool operator()(const std::pair<int, int>& p1, const std::pair<int, int>& p2);

	private:
		std::vector<std::vector<Node*> > tours;
		std::vector<std::vector<int> > matrix;
	};

	std::vector<std::pair<Node*, Node*> > removeVisits(unsigned int count);
	std::vector<std::pair<std::pair<int, int>, double> > rankUsingRelatedness(const std::pair<Node*, Node*>& nodePair, int tourIdx) const;
	double relatedness(const std::pair<Node*, Node*>& n1, int tourIdx1, const std::pair<Node*, Node*>& n2, int tourIdx2) const;
	static bool relatednessSort(const std::pair<std::pair<int, int>, int>& n1, const std::pair<std::pair<int, int>, int>& n2);
	void reinsertPairs(std::vector<std::pair<Node*, Node*> > pairs, int curCosts);
	unsigned int random(unsigned int max);
	int insertAtPosition(std::pair<Node*, Node*> pair, std::pair<int, int> position);
	void removeAtPosition(std::pair<int, int> position);
	std::vector<std::pair<int, int> > getPositionsForPair(std::pair<Node*, Node*> pair);

	Solution bestSolution;
	int bestCosts;
	bool foundBetter;
};

} /* namespace tcbvrp */

#endif /* LNS_H_ */
