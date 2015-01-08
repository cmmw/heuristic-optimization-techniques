/*
 * ACO.cpp
 *
 *  Created on: 05.01.2015
 *      Author: Christian
 */

#include <iostream>
#include <cmath>
#include "ACO.h"

namespace tcbvrp
{

const double INIT_PHERO = 0.5;

const int ANTS = 40;
const int TIMESTEPS = 100;
const double EVAP_RATE = 0.9;

const double ACO_ALPHA = 1;
const double ACO_BETA = 1;

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

	for (int t = 0; t < TIMESTEPS; t++)		//time steps
	{
		std::vector<std::vector<Node*> > tours;
		tours.resize(ANTS);
		for (int k = 0; k < ANTS; k++)		//ants
		{
			std::vector<Node*> tour;
			graph.unvisitNodes();

			//create tour according to probabilities
			Node* n = graph.getZeroNode();
			while (true)
			{
				std::vector<double> p;

				//TODO: build tour and check if it is valid etc. etc. etc.

				//choose next node according to p
				p = calcProbabilites(n, graph.getSupplyNodes());
				n = graph.getSupplyNodes()[1];
				tour.push_back(n);

				//choose next node according to p
				p = calcProbabilites(n, graph.getDemandNodes());
				n = graph.getSupplyNodes()[1];
				tour.push_back(n);

			}
		}

		//Update pheromones
		for (unsigned int i = 0; i < graph.getAdjacencyMatrix().size(); i++)
		{
			for (unsigned int j = 0; j < graph.getAdjacencyMatrix()[i].size(); j++)
			{
				//Reinforce used paths

				for (std::vector<std::vector<Node*> >::iterator tour = tours.begin(); tour != tours.end(); tour++)
				{
					int length = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
					for (std::vector<Node*>::const_iterator it1 = tour->begin(), it2 = it1 + 1; it2 != tour->end(); it1++, it2++)
					{
						pheromones[(*it1)->getId()][(*it2)->getId()] += 1 / (double) length;
					}
				}

				//Evaporation
				for (unsigned int i = 0; i < pheromones.size(); i++)
				{
					for (unsigned int j = 0; j < pheromones[i].size(); j++)
					{
						pheromones[i][j] *= (1 - EVAP_RATE);
					}
				}
			}
		}
	}
}

std::vector<double> ACO::calcProbabilites(Node* node1, const std::vector<Node*>& neighbors)
{
	std::vector<double> p;
	double sum = 0;
	int i = node1->getId();
	p.resize(neighbors.size());

	for (unsigned int n = 0; n < neighbors.size(); n++)
	{
		sum += (pow(pheromones[node1->getId()][neighbors[n]->getId()], ACO_ALPHA) * pow(visibility[node1->getId()][neighbors[n]->getId()], ACO_BETA));
	}

	for (unsigned int n = 0; n < neighbors.size(); n++)
	{
		int j = neighbors[n]->getId();
		if (neighbors[n]->getVisited())
		{
			p[n] = 0;							//set probability of invalid neighbors to 0, TODO check if tour length is violating constraint
		} else
		{
			p[n] = pow(pheromones[i][j], ACO_ALPHA) * pow(visibility[i][j], ACO_BETA) / sum;
		}
	}

	return p;
}

} /* namespace tcbvrp */
