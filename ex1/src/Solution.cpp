/*
 * Solution.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#include <iostream>
#include "Solution.h"
#include "Algorithm.h"
#include "Logger.h"

namespace tcbvrp
{

Solution::Solution(const std::vector<std::vector<int> >& costs) :
		costs(costs)
{
}

Solution::~Solution()
{
}

void Solution::addTours(const std::vector<std::vector<Node*> >& tours)
{
	this->tours = tours;
}

void Solution::addTour(const std::vector<Node*> &tour)
{
	tours.push_back(tour);
}

std::vector<std::vector<Node*> >& Solution::getTours()
{
	return tours;
}

void Solution::clear()
{
	tours.clear();
}

void Solution::setTours(const std::vector<std::vector<Node*> >& tours)
{
	this->tours = tours;
}

int Solution::getNumberOfTours() const
{
	return this->tours.size();
}

void Solution::cleanEmptyTours()
{
	std::vector<std::vector<Node*> >::iterator tour = tours.begin();
	while (tour != tours.end())
	{
		if (tour->empty())
		{
			tour = tours.erase(tour);
		}
		else
		{
			tour++;
		}
	}

	/*if (tours.back().size() == 0) {
	 tours.pop_back();
	 }*/
}

void Solution::printSolution() const
{
	LOG << "";
	LOG << "********* SOLUTION *********";
	LOG << "Tours: ";
	LOG << "";
	int c = 0;
	int totalCosts = 0;
	for (std::vector<std::vector<Node*> >::const_iterator tour = tours.begin(); tour != tours.end(); tour++)
	{
		c++;
		LOG << c << ". Tour:";
		for (std::vector<Node*>::const_iterator it = tour->begin(); it != tour->end(); it++)
		{
			std::string type = ((*it)->getType() == Node::SUPPLY) ? "S" : "D";
			LOG_NOEND << (*it)->getId() << type << ", ";
		}
		int tourCosts = Algorithm::calcTourCosts(*tour, costs);
		LOG << "Costs: " << tourCosts;
		LOG << "";
		totalCosts += tourCosts;
	}
	LOG << "Total costs: " << totalCosts;
}

void Solution::printOfficialSolution() const
{
	int c = 0;
	int totalCosts = 0;
	for (std::vector<std::vector<Node*> >::const_iterator tour = tours.begin(); tour != tours.end(); tour++)
	{
		c++;
		std::cout << "0 ";
		for (std::vector<Node*>::const_iterator it = tour->begin(); it != tour->end(); it++)
		{
			std::cout << (*it)->getId() << " ";
		}
		std::cout << "0" << std::endl;
		int tourCosts = Algorithm::calcTourCosts(*tour, costs);

		totalCosts += tourCosts;
	}
	std::cout << totalCosts << std::endl;
}

int Solution::getTotalCosts() const
{
	int totalCosts = 0;
	for (std::vector<std::vector<Node*> >::const_iterator it = tours.begin(); it != tours.end(); it++)
	{
		if ((*it).size() != 0)
			totalCosts += Algorithm::calcTourCosts(*it, costs);
	}
	return totalCosts;
}

} /* namespace tcbvrp */
