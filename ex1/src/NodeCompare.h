/*
 * NodeCompare.h
 *
 *  Created on: Nov 25, 2014
 *      Author: christian
 */

#ifndef NODECOMPARE_H_
#define NODECOMPARE_H_

#include <vector>
#include "Node.h"

namespace tcbvrp
{

class NodeCompare
{
public:
	NodeCompare(const std::vector<int>& weights);
	virtual ~NodeCompare();

	bool operator()(const Node* n1, const Node* n2);

private:
	std::vector<int> weights;
};

} /* namespace tcbvrp */

#endif /* NODECOMPARE_H_ */
