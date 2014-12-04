/*
 * GRASP.h
 *
 *  Created on: 04.12.2014
 *      Author: Christian
 */

#ifndef GRASP_H_
#define GRASP_H_

#include "Algorithm.h"

namespace tcbvrp
{

class GRASP : public Algorithm
{
public:
	GRASP(Solution* solution, const Graph& graph);
	virtual ~GRASP();
	void solve();
};

} /* namespace tcbvrp */

#endif /* GRASP_H_ */
