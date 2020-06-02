#pragma once

#include <vector>
#include <algorithm>
#include "grid.h"
#include "node.h"

class AStar
{
private:
	Grid& grid;
	Node** nodes;
	std::vector<Node*> priority;

public: AStar(Grid& grid);

public: ~AStar();

public: std::vector<std::pair<int, int>> findPath(int xstart, int ystart, int xend, int yend);

public: void resetNodes();
};