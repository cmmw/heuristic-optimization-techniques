/*
 * ACO.h
 *
 *  Created on: 05.01.2015
 *      Author: Christian
 */

#ifndef ACO_H_
#define ACO_H_

#include "Algorithm.h"

namespace tcbvrp
{

class ACO: public Algorithm
{
public:
	ACO(Solution* solution, const Graph& graph);
	virtual ~ACO();
	void solve();

private:
	std::vector<std::vector<double> > pheromones;
	std::vector<std::vector<std::vector<double> > > ph2;
	std::vector<std::vector<double> > visibility;

	int getBestNodeIdx(std::vector<double> probabilities);

	std::vector<double> calcProbabilites(Node* node1, const std::vector<Node*>& neighbors, unsigned int tourNr);
};

} /* namespace tcbvrp */

#endif /* ACO_H_ */
