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
	const std::vector<std::vector<Node*> >& getTours() const;
	void setTours(const std::vector<std::vector<Node*> >& tours);
	void clear();
	unsigned int getNumberOfTours() const;
	void printSolution() const;

private:
	std::vector<std::vector<Node*> > tours;
	const std::vector<std::vector<int> > costs;
};

} /* namespace tcbvrp */

#endif /* SOLUTION_H_ */
