#include "node.h"

Node::Node(): x(0), y(0), obstacle(false) {}

Node::Node(int x, int y, bool obstacle):
	x(x), y(y), obstacle(obstacle) {}

void Node::update(const Node* parent, const Node* goal)
{
	if (!initialized || parent->local + 1 < local)
	{
		local = parent ? parent->local + 1 : 0;
		global = local + heuristic(goal);
		initialized = true;
		this->parent = parent;
	}
}

double Node::heuristic(const Node* goal) const
{
	int dx = goal->x - x, dy = goal->y - y;
	return sqrt(dx * dx + dy * dy);
}