#pragma once

class Grid
{
public:
	const int width, height;
	bool** grid;

	Grid(int width, int height)
		: width(width), height(height)
	{
		grid = new bool*[height];

		for (int i = 0; i < height; i++)
		{
			grid[i] = new bool[width];

			for (int j = 0; j < width; j++)
				grid[i][j] = false;
		}
	}

	~Grid()
	{
		for (int i = 0; i < height; i++)
			delete[] grid[i];

		delete[] grid;
	}

	bool* const operator[](int index)
	{
		return grid[index];
	}

	bool isInside(int x, int y) const
	{
		return x >= 0 && x < width && y >= 0 && y < height;
	}

	void clear(bool value)
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				grid[i][j] = value;
	}
};