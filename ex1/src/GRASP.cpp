/*
 * GRASP.cpp
 *
 *  Created on: 04.12.2014
 *      Author: Christian
 */

#include <iostream>
#include <climits>

#include "GRASP.h"
#include "LNS.h"
#include "RandConstHeu.h"
#include "Constants.h"
#include "Logger.h"

extern bool quit;

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
	while (i < GRASP_TRIALS && !quit)		//TODO stopping criteria: stop if we don't find a better solution after GRASP_TRIALS tries
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
			LOG << "";
			LOG << "********* Grasp: " << solution->getTotalCosts();
		}
		LOG << "";
		LOG << "---";
		i++;
	}
}

} /* namespace tcbvrp */
