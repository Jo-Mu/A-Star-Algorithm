#pragma once
#include <limits>
#include <vector>

class Node
{
public:
	Node() = default;
	Node(int in_x, int in_y);
	int GetX() const;
	int GetY() const;
public:
	enum State
	{
		Unvisited = 0,
		Visited = 1,
		OnPath = 2,
		Start = 3,
		End = 4,
		Obstacle = 5
	};

	State state = Unvisited;
	float localGoal = INFINITY;
	float globalGoal = INFINITY;
	Node* parent = nullptr;
	std::vector<Node*> neighborNodes;
private:
	int x = 0;
	int y = 0;
};

