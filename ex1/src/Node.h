/*
 * Node.h
 *
 *  Created on: Nov 21, 2014
 *      Author: ixi
 */

#ifndef NODE_H_
#define NODE_H_

namespace tcbvrp {

class Node {

	enum Type
	{
		ZERO,
		SUPPLY,
		DEMAND
	};

public:
	Node(Node::Type type);
	virtual ~Node();

	Node::Type getType() const {
		return type;
	}

private:
	Node::Type type;
};

} /* namespace tcbvrp */

#endif /* NODE_H_ */
