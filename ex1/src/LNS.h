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

	std::vector<std::pair<Node*, Node*> > removeVisits(unsigned int count);
	std::vector<std::pair<std::pair<int, int>, double> > rankUsingRelatedness(const std::pair<Node*, Node*>& nodePair, int tourIdx) const;
	double relatedness(const std::pair<Node*, Node*>& n1, int tourIdx1, const std::pair<Node*, Node*>& n2, int tourIdx2) const;
	static bool sortNodes(const std::pair<std::pair<int, int>, int>& n1, const std::pair<std::pair<int, int>, int>& n2);
	void reinsertPairs(std::vector<std::pair<Node*, Node*> > pairs);
	unsigned int random(unsigned int max);
	void insertAtPosition(std::pair<Node*, Node*> pair, std::pair<int, int> position);
	void removeAtPosition(std::pair<int, int> position);
	std::vector<std::pair<int, int> > getPositionsForPair(std::pair<Node*, Node*> pair);

	Solution bestSolution;
};

} /* namespace tcbvrp */

#endif /* LNS_H_ */
