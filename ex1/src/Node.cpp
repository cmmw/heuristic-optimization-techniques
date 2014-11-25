/*
 * Node.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: ixi
 */

#include "Node.h"

namespace tcbvrp {

Node::Node(Node::Type type, int id) : type(type), id(id), visited(false) {

}

Node::~Node() {
}

} /* namespace tcbvrp */
