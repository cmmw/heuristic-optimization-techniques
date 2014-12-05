/*
 * GRASP.cpp
 *
 *  Created on: 04.12.2014
 *      Author: Christian
 */

#include <iostream>
#include "GRASP.h"
#include "LNS.h"
#include "RandConstHeu.h"
#include "Constants.h"

namespace tcbvrp
{

GRASP::GRASP(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph)
{
}

GRASP::~GRASP()
{
}

void GRASP::solve()
{
	int curCosts;
	int bestCosts = INT_MAX;
	int i = 0;
	while (i < GRASP_TRIALS)		//TODO stopping criteria: stop if we don't find a better solution after GRASP_TRIALS tries
	{
		Solution initSol(graph.getAdjacencyMatrix());
		RandConstHeu rConst(&initSol, graph);
		rConst.solve();
		LNS lns(&initSol, graph);
		lns.solve();
		curCosts = initSol.getTotalCosts();
		if ((curCosts != 0 && curCosts < bestCosts) || bestCosts == INT_MAX)
		{
			*solution = initSol;
			bestCosts = solution->getTotalCosts();
			i = -1;
			std::cout << std::endl << "********* Grasp: " << solution->getTotalCosts();
		}
		std::cout << std::endl << "---" << std::endl;
		i++;
	}
}

} /* namespace tcbvrp */
