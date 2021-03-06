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
public:
	enum Type
	{
		ZERO,
		SUPPLY,
		DEMAND
	};

	Node(Node::Type type, int id);
	virtual ~Node();

	int getId() const {
		return id;
	}

	Node::Type getType() const {
		return type;
	}

	void setVisited(bool val)
	{
		visited = val;
	}

	bool getVisited() const
	{
		return visited;
	}

private:


	Node::Type type;
	int id;
	bool visited;
};

} /* namespace tcbvrp */

#endif /* NODE_H_ */
