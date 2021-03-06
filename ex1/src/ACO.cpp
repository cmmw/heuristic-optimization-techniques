/*
 * ACO.cpp
 *
 *  Created on: 05.01.2015
 *      Author: Christian
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <climits>
#include <cassert>

#include "Constants.h"
#include "ACO.h"
#include "Solution.h"
#include "LNS.h"

extern bool quit;

namespace tcbvrp
{

ACO::ACO(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph)
{
	// Initializes pheremone and visibilty values for all edges
	// pheromones are currently set to INIT_PHERO, visibility is the reverse of the edge costs

	pheromones.resize(graph.getAdjacencyMatrix().size());
	visibility.resize(graph.getAdjacencyMatrix().size());
	for (unsigned int i = 0; i < graph.getAdjacencyMatrix().size(); i++)
	{
		pheromones[i].resize(graph.getAdjacencyMatrix()[i].size());
		visibility[i].resize(graph.getAdjacencyMatrix()[i].size());
		for (unsigned int j = 0; j < graph.getAdjacencyMatrix()[i].size(); j++)
		{
			if (i == j)
			{
				visibility[i][j] = 0;
				pheromones[i][j] = 0;
			} else
			{
				visibility[i][j] = 1 / (double) graph.getAdjacencyMatrix()[i][j];
				pheromones[i][j] = ACO_INIT_PHERO;
			}
		}
	}
}

ACO::~ACO()
{
}

void ACO::solve()
{
	// stores the overall best solution (includes multiple tours)
	std::vector<std::vector<Node*> > bestAntSolution;
	std::vector<std::vector<Node*> > bestSolution;
	int bestSolCosts = INT_MAX;
	int bestCost = INT_MAX;
	for (int t = 0; t < ACO_TIMESTEPS && !quit; t++)		//time steps
	{
		// stores the generated solution for each ant
		std::vector<std::vector<std::vector<Node*> > > solutions;
		for (int k = 0; k < ACO_ANTS; k++)		//ants
		{
			int tourNr = 0;
			std::vector<std::vector<Node*> > tours;
			graph.unvisitNodes();

			//create tour according to probabilities
			int demandNodesLeft = graph.getDemandNodes().size();
			while (demandNodesLeft > 0)
			{
				std::vector<double> p;
				std::vector<Node*> tour;
				Node* n0 = graph.getZeroNode();
				int length = 0;
				int idx = 0;

				if (ACO_phFlag == 'b')
				{
					if (ph2.size() < (unsigned int) tourNr + 1)
					{
						ph2.push_back(pheromones);
					}
				}

				bool noValidNeighboursLeft = false;

				while (!noValidNeighboursLeft)
				{
					Node *n1, *n2;

					std::vector<Node*> validNeighbors;
					for (std::vector<Node*>::const_iterator it = graph.getSupplyNodes().begin(); it != graph.getSupplyNodes().end(); it++)
					{
						if (!(*it)->getVisited())
						{
							validNeighbors.push_back(*it);
						}
					}

					p = calcProbabilites(n0, validNeighbors, tourNr);
					idx = getBestNodeIdx(p);

					// If idx is -1 it means that probability of all the validNeighbours is zero
					// which means that all of them are visited already, there is no possibility
					// to go on with this tour
					if (idx != -1)
					{
						// this assertion makes sure that none of the valid neighbours is visited,
						// some bugs could violate this assertion
						assert((unsigned int ) idx < validNeighbors.size());
						assert(!validNeighbors[idx]->getVisited());

						//choose next node according to p
						n1 = validNeighbors[idx];

						//choose next node according to p
						validNeighbors.clear();
						for (std::vector<Node*>::const_iterator it = graph.getDemandNodes().begin(); it != graph.getDemandNodes().end(); it++)
						{
							if (!(*it)->getVisited())
							{
								validNeighbors.push_back(*it);
							}
						}

						p = calcProbabilites(n1, validNeighbors, tourNr);
						idx = getBestNodeIdx(p);
						if (idx != -1)
						{
							assert((unsigned int ) idx < validNeighbors.size());
							assert(!validNeighbors[idx]->getVisited());

							n2 = validNeighbors[idx];
							length += graph.getAdjacencyMatrix()[n0->getId()][n1->getId()] + graph.getAdjacencyMatrix()[n1->getId()][n2->getId()];
						}
					}
					if (idx == -1 || (unsigned int) (length + graph.getAdjacencyMatrix()[n2->getId()][0]) > graph.getGlobalTimeLimit())
					{
						tourNr++;
						noValidNeighboursLeft = true;
					}
					else
					{
						tour.push_back(n1);
						tour.push_back(n2);
						demandNodesLeft--;
						n1->setVisited(true);
						n2->setVisited(true);
						n0 = n2;
					}
				}

				if (tour.size() != 0)
				{
					tours.push_back(tour);
				}

			}

			// push the tour only if it is valid
			if (tours.size() <= (unsigned int) graph.getNumberOfVehicles() && tours.size() != 0)
			{
				solutions.push_back(tours);
			}
		}

		//Update pheromones

		//Evaporation
		for (unsigned int i = 0; i < graph.getAdjacencyMatrix().size(); i++)
		{
			for (unsigned int j = 0; j < graph.getAdjacencyMatrix()[i].size(); j++)
			{
				switch (ACO_phFlag)
				{
				case 'a':
					case 'c':
					pheromones[i][j] = (1 - ACO_EVAP_RATE) * pheromones[i][j];
					break;
				case 'b':
					for (unsigned int t = 0; t < ph2.size(); t++)
					{
						ph2[t][i][j] = (1 - ACO_EVAP_RATE) * ph2[t][i][j];
					}
					break;
				}
			}
		}

		//Reinforce used paths
		for (std::vector<std::vector<std::vector<Node*> > >::iterator tours = solutions.begin(); tours != solutions.end(); tours++)
		{
			int totalCosts = 0;
			int tourNr = 0;

			for (std::vector<std::vector<Node*> >::iterator tour = tours->begin(); tour != tours->end(); tour++)
			{
				int cost = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
				totalCosts += cost;
			}

			for (std::vector<std::vector<Node*> >::iterator tour = tours->begin(); tour != tours->end(); tour++)
			{
				int tourCost = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
				switch (ACO_phFlag)
				{
				case 'a':
					pheromones[0][(*tour->begin())->getId()] += ACO_Q / (double) totalCosts;
					break;
				case 'b':
					ph2[tourNr][0][(*tour->begin())->getId()] += ACO_Q / (double) totalCosts;
					break;
				case 'c':
					pheromones[0][(*tour->begin())->getId()] += (ACO_Q / (tours->size() * totalCosts)) * ((tourCost - graph.getAdjacencyMatrix()[0][(*tour->begin())->getId()]) / (tour->size() * tourCost));
					break;
				}

				for (std::vector<Node*>::const_iterator it1 = tour->begin(), it2 = it1 + 1; it2 != tour->end(); it1++, it2++)
				{
					switch (ACO_phFlag)
					{
					case 'a':
						pheromones[(*it1)->getId()][(*it2)->getId()] += ACO_Q / (double) totalCosts;
						break;
					case 'b':
						ph2[tourNr][(*it1)->getId()][(*it2)->getId()] += ACO_Q / (double) totalCosts;
						break;
					case 'c':
						pheromones[(*it1)->getId()][(*it2)->getId()] += (ACO_Q / (tours->size() * totalCosts)) * ((tourCost - graph.getAdjacencyMatrix()[(*it1)->getId()][(*it2)->getId()]) / (tour->size() * tourCost));
						break;
					}

					if (it2 + 1 == tour->end())
					{
						switch (ACO_phFlag)
						{
						case 'a':
							pheromones[(*it2)->getId()][0] += ACO_Q / (double) totalCosts;
							break;
						case 'b':
							ph2[tourNr][(*it2)->getId()][0] += ACO_Q / (double) totalCosts;
							break;
						case 'c':
							pheromones[(*it2)->getId()][0] += (ACO_Q / (tours->size() * totalCosts)) * ((tourCost - graph.getAdjacencyMatrix()[(*it2)->getId()][0]) / (tour->size() * tourCost));
							break;
						}
					}
				}
				tourNr++;
			}

			if (bestCost > totalCosts)
			{
				bestCost = totalCosts;
				bestAntSolution = *tours;
			}
		}

		//Daeomon actions (lns)
		Solution sol(graph.getAdjacencyMatrix());
		sol.addTours(bestAntSolution);
		LNS lns(&sol, graph);
		lns.solve();

		if (bestSolCosts == 0 || bestSolCosts > sol.getTotalCosts())
		{
			bestSolCosts = sol.getTotalCosts();
			bestSolution = sol.getTours();
		}

		int totalCosts = 0;
		int tourNr = 0;

		for (std::vector<std::vector<Node*> >::iterator tour = bestSolution.begin(); tour != bestSolution.end(); tour++)
		{
			int cost = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
			totalCosts += cost;
		}

		for (std::vector<std::vector<Node*> >::iterator tour = bestSolution.begin(); tour != bestSolution.end(); tour++)
		{
			int tourCost = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
			switch (ACO_phFlag)
			{
			case 'a':
				pheromones[0][(*tour->begin())->getId()] += ACO_Q / (double) totalCosts;
				break;
			case 'b':
				ph2[tourNr][0][(*tour->begin())->getId()] += ACO_Q / (double) totalCosts;
				break;
			case 'c':
				pheromones[0][(*tour->begin())->getId()] += (ACO_Q / (bestSolution.size() * totalCosts)) * ((tourCost - graph.getAdjacencyMatrix()[0][(*tour->begin())->getId()]) / (tour->size() * tourCost));
				break;
			}

			for (std::vector<Node*>::const_iterator it1 = tour->begin(), it2 = it1 + 1; it2 != tour->end(); it1++, it2++)
			{
				switch (ACO_phFlag)
				{
				case 'a':
					pheromones[(*it1)->getId()][(*it2)->getId()] += ACO_Q / (double) totalCosts;
					break;
				case 'b':
					ph2[tourNr][(*it1)->getId()][(*it2)->getId()] += ACO_Q / (double) totalCosts;
					break;
				case 'c':
					pheromones[(*it1)->getId()][(*it2)->getId()] += (ACO_Q / (bestSolution.size() * totalCosts)) * ((tourCost - graph.getAdjacencyMatrix()[(*it1)->getId()][(*it2)->getId()]) / (tour->size() * tourCost));
					break;
				}

				if (it2 + 1 == tour->end())
				{
					switch (ACO_phFlag)
					{
					case 'a':
						pheromones[(*it2)->getId()][0] += ACO_Q / (double) totalCosts;
						break;
					case 'b':
						ph2[tourNr][(*it2)->getId()][0] += ACO_Q / (double) totalCosts;
						break;
					case 'c':
						pheromones[(*it2)->getId()][0] += (ACO_Q / (bestSolution.size() * totalCosts)) * ((tourCost - graph.getAdjacencyMatrix()[(*it2)->getId()][0]) / (tour->size() * tourCost));
						break;
					}
				}
			}
			tourNr++;
		}
	}

//	this->solution->addTours(bestAntSolution);
	this->solution->addTours(bestSolution);
}

int ACO::getBestNodeIdx(std::vector<double> probabilities)
{
	double probability_sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0);

	if (probability_sum == 0.0)
	{
		// Return -1 if all neighbours have probability 0, which means they are infeasible
		return -1;
	}

	double p = (rand() / (double) (RAND_MAX)) * probability_sum;
	int current = 0;
	while ((p -= probabilities[current]) > 0.00000000001)
	{
		++current;
	}
	return current;
}

std::vector<double> ACO::calcProbabilites(Node* node1, const std::vector<Node*>& neighbors, unsigned int tourNr)
{
	std::vector<double> p;
	double sum = 0;
	int i = node1->getId();
	p.resize(neighbors.size());

	for (unsigned int n = 0; n < neighbors.size(); n++)
	{
		if (neighbors[n]->getVisited())
			continue;
		switch (ACO_phFlag)
		{
		case 'a':
			case 'c':
			sum += (pow(pheromones[node1->getId()][neighbors[n]->getId()], ACO_ALPHA) * pow(visibility[node1->getId()][neighbors[n]->getId()], ACO_BETA));
			break;
		case 'b':
			sum += (pow(ph2[tourNr][node1->getId()][neighbors[n]->getId()], ACO_ALPHA) * pow(visibility[node1->getId()][neighbors[n]->getId()], ACO_BETA));
			break;
		}
	}

	for (unsigned int n = 0; n < neighbors.size(); n++)
	{
		int j = neighbors[n]->getId();
		if (neighbors[n]->getVisited())
		{
			p[n] = 0;
		} else
		{
			switch (ACO_phFlag)
			{
			case 'a':
				case 'c':
				p[n] = pow(pheromones[i][j], ACO_ALPHA) * pow(visibility[i][j], ACO_BETA) / sum;
				break;
			case 'b':
				p[n] = pow(ph2[tourNr][i][j], ACO_ALPHA) * pow(visibility[i][j], ACO_BETA) / sum;
				break;
			}
		}
	}
	return p;
}

} /* namespace tcbvrp */
