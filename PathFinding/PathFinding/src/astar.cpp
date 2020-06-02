#pragma once

#include "astar.h"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

AStar::AStar(Grid& grid)
	: grid(grid)
{
	nodes = new Node * [grid.height];

	for (int y = 0; y < grid.height; y++)
	{
		nodes[y] = new Node[grid.width];

		for (int x = 0; x < grid.width; x++)
		{
			Node& node = nodes[y][x];
			node.x = x;
			node.y = y;
			node.obstacle = grid[y][x];
		}
	}

	priority.reserve(grid.width * grid.height);
}

AStar::~AStar()
{
	for (int i = 0; i < grid.height; i++)
		delete[] nodes[i];

	delete[] nodes;

	priority.~vector();
}

std::vector<std::pair<int, int>> AStar::findPath(int xstart, int ystart, int xend, int yend)
{
	Node* start = &nodes[ystart][xstart];
	Node* end = &nodes[yend][xend];

	if (start->obstacle || end->obstacle)
	{
		resetNodes();
		return std::vector<std::pair<int, int>>();
	}

	start->update(nullptr, end);
	priority.push_back(start);

	auto comparer = [](Node* a, Node* b)
	{
		return a->global < b->global;
	};

	while (!priority.empty())
	{
		std::sort(priority.begin(), priority.end(), comparer);
		Node* node = priority[0];

		for (int i = 0; i < 4; i++)
		{
			int x = i + 3 - 3 * int(i / 3.0f + 1.5f) + node->x;
			int y = (i + 1) / 2 - 1 + node->y;
			
			if (x >= 0 && y >= 0 && x < grid.width && y < grid.height && !grid[y][x])
			{
				Node* next = &nodes[y][x];

				if (!next->initialized)
					priority.push_back(next);

				next->update(node, end);
			}
		}

		node->explored = true;
		priority.erase(priority.begin());
	}

	std::vector<std::pair<int, int>> path;
	const Node* current = end;

	while (current != nullptr)
	{
		path.emplace_back(current->x, current->y);
		current = current->parent;
	}

	std::reverse(path.begin(), path.end());
	resetNodes();
	priority.clear();

	return path;
}

void AStar::resetNodes()
{
	for (int y = 0; y < grid.height; y++)
		for (int x = 0; x < grid.width; x++)
		{
			Node& node = nodes[y][x];
			node.x = x;
			node.y = y;
			node.obstacle = grid[y][x];
			node.parent = nullptr;
			node.global = 0;
			node.local = 0;
			node.initialized = false;
			node.explored = false;
		}
}