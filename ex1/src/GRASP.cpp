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
	int i = 0;
	while (i < 5)		//TODO stopping criteria: stop if we don't find a better solution after 5 tries
	{
		Solution initSol(graph.getAdjacencyMatrix());
		RandConstHeu rConst(&initSol, graph);
		rConst.solve();
		LNS lns(&initSol, graph);
		lns.solve();
		if (initSol.getTotalCosts() < solution->getTotalCosts() || solution->getTotalCosts() == 0)
		{
			*solution = initSol;
			i = -1;
			std::cout << std::endl << "********* Grasp: " << solution->getTotalCosts() << std::endl;
		}
		i++;
	}
}

} /* namespace tcbvrp */
