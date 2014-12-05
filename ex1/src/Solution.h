/*
 * Solution.h
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <vector>
#include "Node.h"

namespace tcbvrp
{

class Solution
{
public:
	Solution(const std::vector<std::vector<int> >& costs);
	virtual ~Solution();

	void addTour(const std::vector<Node*> &tour);
	std::vector<std::vector<Node*> >& getTours();
	void setTours(const std::vector<std::vector<Node*> >& tours);
	void clear();
	int getNumberOfTours() const;
	void printSolution() const;
	void printOfficialSolution() const;
	int getTotalCosts() const;
	void cleanEmptyTours();

private:
	std::vector<std::vector<Node*> > tours;
	std::vector<std::vector<int> > costs;
};

} /* namespace tcbvrp */

#endif /* SOLUTION_H_ */
