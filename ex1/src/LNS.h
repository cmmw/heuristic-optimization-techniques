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

namespace tcbvrp {

class LNS: public Algorithm {
public:
	LNS(Solution* solution, const Graph& graph);
	virtual ~LNS();

	void solve();

private:
	std::vector< std::pair<Node*, Node*> > removeVisits(int count);
	void reinsertPairs(std::vector<std::pair<Node*, Node*> > pairs);


	void insertAtPosition(std::pair<Node*, Node*> pair, std::pair<int, int> position);
	void removeAtPosition(std::pair<int, int> position);

	std::vector< std::pair<int, int> > getPositionsForPair(std::pair<Node*, Node*> pair);

	Solution bestSolution;
};

} /* namespace tcbvrp */

#endif /* LNS_H_ */
