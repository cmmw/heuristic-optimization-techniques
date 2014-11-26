/*
 * Solution.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#include "Solution.h"

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

const std::vector<std::vector<Node*> >& Solution::getTours() const
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

unsigned int Solution::getNumberOfTours() const
{
	return this->tours.size();
}

} /* namespace tcbvrp */
