/*
 * LNS.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: ixi
 */

#include "LNS.h"

#include "Constants.h"

namespace tcbvrp {

LNS::LNS(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph) {

}

LNS::~LNS() {
}

void LNS::solve() {
	// the overall search procedure
	int removes = START_REMOVES;
	int trials = 0;

	while (removes <= REMOVE_LIMIT) {
		if (trials == TRIALS_PER_COUNT) {
			removes++;
			trials = 0;
		}

		// Save old solution
		Solution old = *solution;

		// Choose Visits to remove


		// Reinsert the visits


		if (true) {
			*solution = old;
		}
	}




}

std::vector<int> LNS::removeVisits(int count) {
	std::vector<int> temp;
	return temp;
}

void LNS::reinsertVisits(std::vector<int> nodeIds) {
}

} /* namespace tcbvrp */
