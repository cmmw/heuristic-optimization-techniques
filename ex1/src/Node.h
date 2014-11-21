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
	Node(Node::Type type, int id);
	virtual ~Node();

	int getId() const {
		return id;
	}

	Node::Type getType() const {
		return type;
	}

private:
	Node::Type type;
	int id;
};

} /* namespace tcbvrp */

#endif /* NODE_H_ */
