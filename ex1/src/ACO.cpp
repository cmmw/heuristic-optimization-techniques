/*
 * ACO.cpp
 *
 *  Created on: 05.01.2015
 *      Author: Christian
 */

#include <iostream>
#include <cmath>
#include "ACO.h"
#include <cstdlib>
#include <numeric>
#include <climits>
#include <cassert>

namespace tcbvrp
{

const double INIT_PHERO = 1;

const int ANTS = 100;
const int TIMESTEPS = 50;
const double EVAP_RATE = 0.2;

const double ACO_ALPHA = 1;
const double ACO_BETA = 1;

bool usePh2 = false;

ACO::ACO(Solution* solution, const Graph& graph) :
		Algorithm(solution, graph)
{
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
				pheromones[i][j] = INIT_PHERO;
			}
		}
	}
}

ACO::~ACO()
{
}

void ACO::solve()
{
	std::vector<std::vector<Node*> > bestTours;
	int bestCost = INT_MAX;
	for (int t = 0; t < TIMESTEPS; t++)		//time steps
	{
		std::vector<std::vector<std::vector<Node*> > > solutions;
		for (int k = 0; k < ANTS; k++)		//ants
		{
			int tourNr = 0;
			std::vector<std::vector<Node*> > tours;
			graph.unvisitNodes();

			//create tour according to probabilities
			while (true)
			{
				std::vector<double> p;
				std::vector<Node*> tour;
				Node* n0 = graph.getZeroNode();
				int length = 0;
				int idx = 0;

				if (usePh2)
				{
					if (ph2.size() < (unsigned int) tourNr + 1)
					{
						ph2.push_back(pheromones);
					}
				}

				while (true)
				{
					Node *n1, *n2;

					std::vector<Node*> validNeighbors;
					for (std::vector<Node*>::const_iterator it = graph.getSupplyNodes().begin(); it != graph.getSupplyNodes().end(); it++)
					{
						if ((*it)->getVisited())
							continue;
						validNeighbors.push_back(*it);
					}
					p = calcProbabilites(n0, validNeighbors, tourNr);

					idx = getBestNodeIdx(p);
					if (idx != -1)
					{
						assert((unsigned int ) idx < validNeighbors.size());
						assert(!validNeighbors[idx]->getVisited());

						//choose next node according to p
						n1 = validNeighbors[idx];

						//choose next node according to p
						validNeighbors.clear();
						for (std::vector<Node*>::const_iterator it = graph.getDemandNodes().begin(); it != graph.getDemandNodes().end(); it++)
						{
							if ((*it)->getVisited())
								continue;
							validNeighbors.push_back(*it);
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
					if ((unsigned int) (length + graph.getAdjacencyMatrix()[n2->getId()][0]) > graph.getGlobalTimeLimit() || idx == -1)
					{
						tourNr++;
						break;
					}
					tour.push_back(n1);
					tour.push_back(n2);
					n1->setVisited(true);
					n2->setVisited(true);
					n0 = n2;
				}

				if (tour.size() != 0)
				{
					tours.push_back(tour);
				}

				if (idx == -1)
					break;
			}

			if (tours.size() <= (unsigned int) graph.getNumberOfVehicles() && tours.size() != 0)
			{
				solutions.push_back(tours);
			}

		}

		//Update pheromones

		//Reinforce used paths
		for (std::vector<std::vector<std::vector<Node*> > >::iterator tours = solutions.begin(); tours != solutions.end(); tours++)
		{
			int totalCosts = 0;
			int tourNr = 0;
			for (std::vector<std::vector<Node*> >::iterator tour = tours->begin(); tour != tours->end(); tour++)
			{
				int length = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
				totalCosts += length;

				if (usePh2)
					ph2[tourNr][0][(*tour->begin())->getId()] += 1 / (double) length;
				else
					pheromones[0][(*tour->begin())->getId()] += 1 / (double) length;

				for (std::vector<Node*>::const_iterator it1 = tour->begin(), it2 = it1 + 1; it2 != tour->end(); it1++, it2++)
				{
					if (usePh2)
						ph2[tourNr][(*it1)->getId()][(*it2)->getId()] += 1 / (double) length;
					else
						pheromones[(*it1)->getId()][(*it2)->getId()] += 1 / (double) length;

					if (it2 + 1 == tour->end())
					{
						if (usePh2)
							ph2[tourNr][(*it2)->getId()][0] += 1 / (double) length;
						else
							pheromones[(*it2)->getId()][0] += 1 / (double) length;
					}
				}
				tourNr++;
			}

			if (bestCost > totalCosts)
			{
				bestCost = totalCosts;
				bestTours = *tours;
			}
		}

		//Evaporation
		for (unsigned int i = 0; i < graph.getAdjacencyMatrix().size(); i++)
		{
			for (unsigned int j = 0; j < graph.getAdjacencyMatrix()[i].size(); j++)
			{
				if (usePh2)
				{
					for (unsigned int t = 0; t < ph2.size(); t++)
					{
						ph2[t][i][j] = (1 - EVAP_RATE) * ph2[t][i][j];
					}
				} else
				{
					pheromones[i][j] = (1 - EVAP_RATE) * pheromones[i][j];
				}
			}

		}
	}

	this->solution->addTours(bestTours);
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
		if (usePh2)
			sum += (pow(ph2[tourNr][node1->getId()][neighbors[n]->getId()], ACO_ALPHA) * pow(visibility[node1->getId()][neighbors[n]->getId()], ACO_BETA));
		else
			sum += (pow(pheromones[node1->getId()][neighbors[n]->getId()], ACO_ALPHA) * pow(visibility[node1->getId()][neighbors[n]->getId()], ACO_BETA));
	}

	for (unsigned int n = 0; n < neighbors.size(); n++)
	{
		int j = neighbors[n]->getId();
		if (neighbors[n]->getVisited())
		{
			p[n] = 0;
		} else
		{
			if (usePh2)
				p[n] = pow(ph2[tourNr][i][j], ACO_ALPHA) * pow(visibility[i][j], ACO_BETA) / sum;
			else
				p[n] = pow(pheromones[i][j], ACO_ALPHA) * pow(visibility[i][j], ACO_BETA) / sum;
		}
	}

	return p;
}

} /* namespace tcbvrp */
