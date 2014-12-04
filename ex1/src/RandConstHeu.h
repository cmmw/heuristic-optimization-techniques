/*
 * ConsHeu.h
 *
 *  Created on: Nov 24, 2014
 *      Author: christian
 */

#ifndef RANDCONSTHEU_H_
#define RANDCONSTHEU_H_

#include "Algorithm.h"

namespace tcbvrp
{

class RandConstHeu: public Algorithm
{
public:
	RandConstHeu(Solution* solution, const Graph& graph);
	virtual ~RandConstHeu();

	void solve();

};

} /* namespace tcbvrp */

#endif /* RANDCONSTHEU_H_ */
