#pragma once

#include <cmath>

class Node
{
	friend class AStar;

public:
	int x, y;
	bool obstacle;
	const Node* parent = nullptr;

private:
	double global = 0, local = 0;
	bool initialized = false;
	bool explored = false;

public: Node();

public: Node(int x, int y, bool obstacle = false);

public: void update(const Node* parent, const Node* goal);

private: double heuristic(const Node* goal) const;
};