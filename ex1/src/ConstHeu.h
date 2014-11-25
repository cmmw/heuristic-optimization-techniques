/*
 * ConsHeu.h
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#ifndef SRC_CONSTHEU_H_
#define SRC_CONSTHEU_H_

#include "Algorithm.h"

namespace tcbvrp
{

class ConstHeu: public Algorithm
{
public:
	ConstHeu(Solution* solution, const Graph& graph);
	virtual ~ConstHeu();

	void solve();

};

} /* namespace tcbvrp */

#endif /* SRC_CONSTHEU_H_ */
