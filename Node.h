#pragma once
#include <limits>

class Node
{
public:
	Node() = default;

	Node(int in_x, int in_y);

	enum State
	{
		Unvisited = 0,
		OnPath = 1,
		Obstacle = 2
	};

	State state = Unvisited;
	float localGoal = INFINITY;
	float globalGoal = INFINITY;
	Node* parent = nullptr;

private:
	int x = 0;
	int y = 0;
};

