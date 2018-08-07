// A-Star Algorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Node.h"
#include "math.h"
#include <limits>
#include <list>

Node* CreateNodeMap(int mapWidth, int mapHeight)
{
	Node* nodeMap = new Node[mapWidth * mapHeight];

	//Initializing nodes in array.
	for(int x = 0; x < mapWidth; x++)
	{
		for(int y = 0; y < mapHeight; y++)
		{
			nodeMap[(y * mapWidth) + x] = Node(x, y);
		}
	}

	//Add neighboring nodes to each nodes neighborNodes vector.
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			if (y > 0) 
			{
				nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[(y - 1) * mapWidth + x]);
				
				if(x > 0)
				{
					nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[(y - 1) * mapWidth + (x - 1)]);
				}

				if(x  < mapWidth - 1)
				{
					nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[(y - 1) * mapWidth + (x + 1)]);
				}
			}
			
			if(y < mapHeight - 1 )
			{
				nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[(y + 1) * mapWidth + x]);

				if( x > 0)
				{
					nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[(y + 1) * mapWidth + (x - 1)]);
				}

				if(x < mapWidth - 1)
				{
					nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[(y + 1) * mapWidth + (x + 1)]);
				}
			}

			if(x > 0)
			{
				nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[y * mapWidth + (x - 1)]);
			}

			if(x < mapHeight - 1)
			{
				nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[y * mapWidth + (x + 1)]);
			}
		}
	}

	return nodeMap;
}

float DistanceBetweenNodes(const Node* a, const Node* b)
{
	return sqrtf((float)((a->GetX() - b->GetX())*(a->GetX() - b->GetX())) + (float)((a->GetY() - b->GetY()) * (a->GetY() - b->GetY())));
}

void DrawAStar(const Node* nodeMap, int mapWidth, int mapHeight)
{
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			switch (nodeMap[(y * mapWidth) + x].state)
			{
			case Node::State::Unvisited:
				std::cout << "- ";
				break;
			case Node::State::Visited:
				std::cout << "O ";
				break;
			case Node::State::OnPath:
				std::cout << "* ";
				break;
			case Node::State::Obstacle:
				std::cout << "X ";
				break;
			case Node::State::Start:
				std::cout << "S ";
				break;
			case Node::State::End:
				std::cout << "F ";
				break;
			default:
				break;
			}
		}

		std::cout << std::endl;
	}
}

void SolveAStar(Node* nodeMap, int mapWidth, int mapHeight)
{
	//Default start and end node values so they are not nullptr.
	Node* startNode = &nodeMap[0];
	Node* endNode = &nodeMap[((mapHeight / 2) * mapWidth) + (mapWidth / 2)];

	//Reset all nodes in nodeMap.
	for(int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			if (nodeMap[(y * mapWidth) + x].state != Node::State::Obstacle)
			{
				nodeMap[(y * mapWidth) + x].state = Node::State::Unvisited;
			}
			nodeMap[(y * mapWidth) + x].localGoal = INFINITY;
			nodeMap[(y * mapWidth) + x].globalGoal = INFINITY;
			nodeMap[(y * mapWidth) + x].parent = nullptr;
		}
	}

	bool gettingStartNode = true;

	while(gettingStartNode) 
	{
		std::cout << "Enter Starting Node X: ";

		int startX;

		std::cin >> startX;

		if(startX < 0 || startX >= mapWidth)
		{
			std::cout << "Out of Bounds!" << std::endl;
			continue;
		}

		std::cout << "Enter Starting Node Y: ";

		int startY;
		
		std::cin >> startY;

		if(startY < 0 || startY >= mapHeight)
		{
			std::cout << "Out of Bounds!" << std::endl;
		}
		else if(startX == endNode->GetX() && startY == endNode->GetY())
		{
			std::cout << "Start Node location is the same as End Node, Enter new coordinates!" << std::endl;
		}
		else
		{
			startNode = &nodeMap[(startY * mapWidth) + startX];
			gettingStartNode = false;
		}
	}

	bool gettingEndNode = true;

	while(gettingEndNode)
	{
		std::cout << "Enter End Node X: ";

		int endX;

		std::cin >> endX;

		if(endX < 0 || endX >= mapWidth)
		{
			std::cout << "Out of Bounds!" << std::endl;
			continue;
		}

		std::cout << "Enter End Node Y: ";

		int endY;

		std::cin >> endY;

		if(endY < 0 || endY >= mapHeight)
		{
			std::cout << "Out of Bounds!" << std::endl;
		}
		else if(endX == startNode->GetX() && endY == startNode->GetY())
		{
			std::cout << "End Node location is the same as Start Node, Enter new coordinates!" << std::endl;
		}
		else
		{
			endNode = &nodeMap[(endY * mapWidth) + endX];
			gettingEndNode = false;
		}
	}

	//Initialize proper settings for start node as well as make the current node to evaluate the starting node.
	startNode->localGoal = 0.0f;
	startNode->globalGoal = DistanceBetweenNodes(startNode, endNode);
	Node* currentNode = startNode;

	//Add starting node to the list to be evalutated to make sure that it gets tested first.
	std::list<Node*> nodesToBeEvaluated;
	nodesToBeEvaluated.push_back(startNode);

	while (!nodesToBeEvaluated.empty())
	{
		//Sort list by lowest globalGoal first so that the potential shortest path is always evaluated first.
		nodesToBeEvaluated.sort([](const Node* lhs, const Node* rhs) {return lhs->globalGoal < rhs->globalGoal; });

		//If the first node in the list has been visited then remove it.
		while(!nodesToBeEvaluated.empty() && (nodesToBeEvaluated.front()->state == Node::State::Visited))
		{
			nodesToBeEvaluated.pop_front();
		}

		if(nodesToBeEvaluated.empty())
		{
			break;
		}

		//Evaluating the front node in the list so set it's State to Visited.
		currentNode = nodesToBeEvaluated.front();
		currentNode->state = Node::State::Visited;

		//Check all neighbors of the current node.
		for(auto nodeNeighbor : currentNode->neighborNodes)
		{
			//If the neighboring node is has not been visited and is not an obstacle then add it to the nodes to evaluate list
			if(nodeNeighbor->state == Node::State::Unvisited)
			{
				nodesToBeEvaluated.push_back(nodeNeighbor);
				//Calculate local goal of current node to neighbor
				float calculatedLocalGoal = DistanceBetweenNodes(currentNode, nodeNeighbor) + currentNode->localGoal;
				
				//If the calculated local goal is less than the neighbors local goal then change then...
				//Change parent of neighbor to current node.
				//Change neighbor local goal to calculated goal.
				//Change global goal of neighbor to the distance between the neighbor node and end node + new local goal.
				if(calculatedLocalGoal < nodeNeighbor->localGoal)
				{
					nodeNeighbor->parent = currentNode;
					nodeNeighbor->localGoal = calculatedLocalGoal;
					nodeNeighbor->globalGoal = DistanceBetweenNodes(nodeNeighbor, endNode) + nodeNeighbor->localGoal;
				}
			}
		}
	}

	//Provided that the end node has a parent, all nodes on the shortest path between the start and end node will have their states changed to OnPath.
	//Start and end nodes will have their states changed to 'Start' and 'End' respectively.
	//If the end node doesn't have a parent then a path could not be found.
	if(endNode->parent != nullptr)
	{
		endNode->state = Node::State::End;
		endNode->parent->state = Node::State::OnPath;
		Node* currentPathNode = endNode->parent;

		while(currentPathNode->parent != nullptr)
		{
			currentPathNode->parent->state = Node::State::OnPath;
			currentPathNode = currentPathNode->parent;
		}

		currentPathNode->state = Node::State::Start;
	}

	system("CLS");
	DrawAStar(nodeMap, mapWidth, mapHeight);
}

int main()
{
	const int minMapSideLength = 2;
	const int maxMapSideLength = 30;
	Node* nodeMap = nullptr;
	int mapWidth;
	int mapHeight;
	bool creatingMap = true;

	while (creatingMap)
	{
		std::cout << "Enter Map Width (in range of " << minMapSideLength << " to " << maxMapSideLength << "): ";
		std::cin >> mapWidth;

		if(mapWidth < minMapSideLength || mapWidth > maxMapSideLength)
		{
			std::cout << "Invalid Map Width. Please enter side lengths between in the range of " << minMapSideLength << " to " << maxMapSideLength << "." << std::endl;
			continue;
		}

		std::cout << "Enter Map Height (in range of " << minMapSideLength << " to " << maxMapSideLength << "): ";
		std::cin >> mapHeight;

		if (mapHeight < minMapSideLength || mapHeight > maxMapSideLength)
		{
			std::cout << "Invalid Map Height. Please enter side lengths between in the range of " << minMapSideLength << " to " << maxMapSideLength << "." << std::endl;
			continue;
		}
		else
		{
			nodeMap = CreateNodeMap(mapWidth, mapHeight);
			creatingMap = false;
		}
	}

	SolveAStar(nodeMap, mapWidth, mapHeight);

	delete[] nodeMap;

    return 0;
}

