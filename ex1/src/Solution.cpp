/*
 * Solution.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#include <iostream>
#include "Solution.h"
#include "Algorithm.h"

namespace tcbvrp
{

Solution::Solution(const std::vector<std::vector<int> >& costs) :
		costs(costs)
{
}

Solution::~Solution()
{
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

void Solution::printSolution() const
{
	std::cout << "Tours: " << std::endl << std::endl;
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
		int tourCosts = Algorithm::calcTourCosts(*tour, costs);
		std::cout << std::endl << "Costs: " << tourCosts << std::endl << std::endl;
		totalCosts += tourCosts;
	}
	std::cout << "Total costs: " << totalCosts << std::endl;
}

int Solution::getTotalCosts() const
{
	int totalCosts = 0;
	for (std::vector<std::vector<Node*> >::const_iterator it = tours.begin(); it != tours.end(); it++)
	{
		totalCosts += Algorithm::calcTourCosts(*it, costs);
	}
	return totalCosts;
}

} /* namespace tcbvrp */
