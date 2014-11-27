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
	void reinsertVisits(std::vector<std::pair<Node*, Node*> > nodeIds);
	unsigned int random(unsigned int max);
	};

} /* namespace tcbvrp */

#endif /* LNS_H_ */
