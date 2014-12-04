/*
 * NodeCompare.cpp
 *
 *  Created on: Nov 25, 2014
 *      Author: christian
 */

#include "NodeCompare.h"

namespace tcbvrp
{

NodeCompare::NodeCompare(const std::vector<int>& weights) :
		weights(weights)
{
}

NodeCompare::~NodeCompare()
{
}

bool NodeCompare::operator()(const Node* n1, const Node* n2)
{
	return weights[n1->getId()] < weights[n2->getId()];
}

} /* namespace tcbvrp */
