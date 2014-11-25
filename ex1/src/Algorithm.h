/*
 * Algorithm.h
 *
 *  Created on: Nov 21, 2014
 *      Author: christian
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "Solution.h"
#include "Graph.h"

namespace tcbvrp
{

class Algorithm
{
public:
	Algorithm(Solution* solution, const Graph& graph);
	virtual ~Algorithm();

	virtual void solve() = 0;

protected:
	Solution* solution;
	const Graph& graph;

};

} /* namespace tcbvrp */

#endif /* ALGORITHM_H_ */
