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
	std::vector<std::vector<Node*> > bestTours;
	int bestCost = INT_MAX;

	for (int t = 0; t < TIMESTEPS; t++)		//time steps
	{
		std::vector<std::vector<std::vector<Node*> > > solutions;
		for (int k = 0; k < ANTS; k++)		//ants
		{
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
				while (true)
				{
					Node *n1, *n2;

					p = calcProbabilites(n0, graph.getSupplyNodes());

					idx = getBestNodeIdx(p);
					if (idx != -1)
					{



						//choose next node according to p
						n1 = graph.getSupplyNodes()[idx];

						std::cout << "Is VISITED: " << n1->getVisited() << std::endl;
						std::cout << "probability: " << p[idx] << std::endl;

						//choose next node according to p
						p = calcProbabilites(n1, graph.getDemandNodes());
						idx = getBestNodeIdx(p);

						if (idx != -1)
						{
							n2 = graph.getDemandNodes()[idx];

							std::cout << "Is VISITED: " << n2->getVisited() << std::endl;
							std::cout << "probability: " << p[idx] << std::endl;

							length += graph.getAdjacencyMatrix()[n0->getId()][n1->getId()] + graph.getAdjacencyMatrix()[n1->getId()][n2->getId()];
						}
					}
					if ((unsigned int) (length + graph.getAdjacencyMatrix()[n2->getId()][0]) > graph.getGlobalTimeLimit() || idx == -1)
					{
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

				int c = 0;
				int totalCosts = 0;
				for (std::vector<std::vector<Node*> >::const_iterator tour = tours.begin(); tour != tours.end(); tour++)
				{
					c++;
					std::cout << c << ". Tour:" << std::endl;
					for (std::vector<Node*>::const_iterator it = tour->begin(); it != tour->end(); it++)
					{
						std::string type = ((*it)->getType() == Node::SUPPLY) ? "S" : "D";
						std::cout << (*it)->getId() << type << ", ";
					}
					int tourCosts = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
					std::cout << "Costs: " << tourCosts << std::endl;
					std::cout << "" << std::endl;
					totalCosts += tourCosts;
				}
				std::cout << "Total costs: " << totalCosts << std::endl;
			}

		}

		//Update pheromones

		//Reinforce used paths

		for (std::vector<std::vector<std::vector<Node*> > >::iterator tours = solutions.begin(); tours != solutions.end(); tours++)
		{
			///std::cout << "Tours size: " << tours->size() << std::endl;
			int totalCosts = 0;
			for (std::vector<std::vector<Node*> >::iterator tour = tours->begin(); tour != tours->end(); tour++)
			{
				int length = Algorithm::calcTourCosts(*tour, graph.getAdjacencyMatrix());
				totalCosts += length;
				pheromones[0][(*tour->begin())->getId()] += 1 / (double) length;
				for (std::vector<Node*>::const_iterator it1 = tour->begin(), it2 = it1 + 1; it2 != tour->end(); it1++, it2++)
				{
					pheromones[(*it1)->getId()][(*it2)->getId()] += 1 / (double) length;
					if (it2 + 1 == tour->end())
						pheromones[(*it2)->getId()][0] += 1 / (double) length;
				}
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
				for (unsigned int i = 0; i < pheromones.size(); i++)
				{
					for (unsigned int j = 0; j < pheromones[i].size(); j++)
					{
						pheromones[i][j] *= (1 - EVAP_RATE);
						pheromones[i][j] += 0.0001;
					}
				}
			}
		}
	}

	this->solution->addTours(bestTours);
}

int ACO::getBestNodeIdx(std::vector<double> probabilities)
{
	std::cout << "probabilities: " << std::endl;

	for (std::vector<double>::iterator it = probabilities.begin(); it != probabilities.end(); it++)
	{
		std::cout << *it << " ";
	}

	double probability_sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0);
	std::cout << "probability sum: " << probability_sum << std::endl;

	if (probability_sum == 0.0)
	{
		// Return -1 if all neighbours have probability 0, which means they are infeasible
		return -1;
	}

	double p = (rand() / static_cast<double>(RAND_MAX)) * probability_sum;
	int current = 0;
	while ((p -= probabilities[current]) > 0)
	{
		++current;
	}
	return current;
}

std::vector<double> ACO::calcProbabilites(Node* node1, const std::vector<Node*>& neighbors)
{
	std::vector<double> p;
	double sum = 0;
	int i = node1->getId();
	p.resize(neighbors.size());

	for (unsigned int n = 0; n < neighbors.size(); n++)
	{
		if (!neighbors[n]->getVisited())
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
			if (sum == 0) {
				std::cout << "Visited: " << neighbors[n]->getVisited() << std::endl;
				std::cout << "Pheromone: " << pheromones[node1->getId()][neighbors[n]->getId()] << std::endl;
				std::cout << "Alpha: " << ACO_ALPHA << std::endl;
				std::cout << "visibility: " << visibility[node1->getId()][neighbors[n]->getId()] << std::endl;
				std::cout << "ACO_BETA" << ACO_BETA << std::endl;

			}
			p[n] = pow(pheromones[i][j], ACO_ALPHA) * pow(visibility[i][j], ACO_BETA) / sum;
		}
	}

	return p;
}

} /* namespace tcbvrp */
