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

	/*Helper functions*/
	static unsigned int calcTourCosts(const std::vector<Node*> &tour, const std::vector<std::vector<int> >& costs);
	/*Sorts nodes in descending order corresponding to the costs in costRow*/
	static void sortNeighborsByAscEdgeCosts(std::vector<Node*> &nodes, const std::vector<int>& costRow);
	/*return random number between 0 and max*/
	static unsigned int random(unsigned int max);

protected:
	Solution* solution;
	const Graph& graph;

};

} /* namespace tcbvrp */

#endif /* ALGORITHM_H_ */
