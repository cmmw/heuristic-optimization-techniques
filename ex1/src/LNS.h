/*
 * LNS.h
 *
 *  Created on: Nov 27, 2014
 *      Author: ixi
 */

#ifndef LNS_H_
#define LNS_H_

#include <vector>
#include "Algorithm.h"

namespace tcbvrp {

class LNS: public Algorithm {
public:
	LNS(Solution* solution, const Graph& graph);
	virtual ~LNS();

	void solve();

private:
	std::vector<int> removeVisits(int count);
	void reinsertVisits(std::vector<int> nodeIds);
};

} /* namespace tcbvrp */

#endif /* LNS_H_ */
