// A-Star-Algorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Node.h"
#include "math.h"
#include <limits>
#include <list>
#include <string>
#include "conio.h"

Node* CreateDiagonalNodeMap(int mapWidth, int mapHeight)
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

Node* CreateNodeMap(int mapWidth, int mapHeight)
{
	Node* nodeMap = new Node[mapWidth * mapHeight];

	//Initializing nodes in array.
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
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
			}

			if (y < mapHeight - 1)
			{
				nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[(y + 1) * mapWidth + x]);
			}

			if (x > 0)
			{
				nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[y * mapWidth + (x - 1)]);
			}

			if (x < mapHeight - 1)
			{
				nodeMap[(y * mapWidth) + x].neighborNodes.push_back(&nodeMap[y * mapWidth + (x + 1)]);
			}
		}
	}

	return nodeMap;
}

//Checks case of a cin fail in input, mostly for integer inputs, and resets the input stream
bool CheckAndManageCinFail()
{
	if(std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		return true;
	}

	return false;
}

float DistanceBetweenNodes(const Node* a, const Node* b)
{
	return sqrtf((float)((a->GetX() - b->GetX())*(a->GetX() - b->GetX())) + (float)((a->GetY() - b->GetY()) * (a->GetY() - b->GetY())));
}

void DrawAStar(const Node* nodeMap, int mapWidth, int mapHeight)
{
	for (int y = -1; y < mapHeight; y++)
	{
		if(y == -1)
		{
			std::cout << "  ";
		}
		else
		{
			std::cout << y << " ";
		}

		for (int x = 0; x < mapWidth; x++)
		{
			if (y == -1) 
			{
				std::cout << x << " ";
			}
			else 
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
		}

		std::cout << std::endl;
	}
}

void SolveAStar(Node* nodeMap, int mapWidth, int mapHeight)
{
	//Default start and end node values so they are not nullptr.
	Node* startNode = &nodeMap[0];
	Node* endNode = &nodeMap[((mapHeight / 2) * mapWidth) + (mapWidth / 2)];
	bool solvingAStar = true;

	while (solvingAStar) 
	{
		//Reset all nodes in nodeMap. Obstacles will stay obstacles uless the user toggles them off.
		for (int x = 0; x < mapWidth; x++)
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
			while (!nodesToBeEvaluated.empty() && (nodesToBeEvaluated.front()->state == Node::State::Visited))
			{
				nodesToBeEvaluated.pop_front();
			}

			if (nodesToBeEvaluated.empty())
			{
				break;
			}

			//Evaluating the front node in the list so set it's State to Visited.
			currentNode = nodesToBeEvaluated.front();
			currentNode->state = Node::State::Visited;

			//Check all neighbors of the current node.
			for (auto nodeNeighbor : currentNode->neighborNodes)
			{
				//If the neighboring node is has not been visited and is not an obstacle then add it to the nodes to evaluate list
				if (nodeNeighbor->state == Node::State::Unvisited)
				{
					nodesToBeEvaluated.push_back(nodeNeighbor);
					//Calculate local goal of current node to neighbor
					float calculatedLocalGoal = DistanceBetweenNodes(currentNode, nodeNeighbor) + currentNode->localGoal;

					//If the calculated local goal is less than the neighbors local goal then change then...
					//Change parent of neighbor to current node.
					//Change neighbor local goal to calculated goal.
					//Change global goal of neighbor to the distance between the neighbor node and end node + new local goal.
					if (calculatedLocalGoal < nodeNeighbor->localGoal)
					{
						nodeNeighbor->parent = currentNode;
						nodeNeighbor->localGoal = calculatedLocalGoal;
						nodeNeighbor->globalGoal = DistanceBetweenNodes(nodeNeighbor, endNode) + nodeNeighbor->localGoal;
					}
				}
			}
		}

		//In the case that A* is not solved the start/end nodes will still show.
		endNode->state = Node::State::End;
		startNode->state = Node::State::Start;

		//Provided that the end node has a parent, all nodes on the shortest path between the start and end node will have their states changed to OnPath.
		//If the end node doesn't have a parent then a path could not be found.
		if (endNode->parent != nullptr)
		{
			endNode->parent->state = Node::State::OnPath;
			Node* currentPathNode = endNode->parent;

			while (currentPathNode->parent != nullptr)
			{
				currentPathNode->parent->state = Node::State::OnPath;
				currentPathNode = currentPathNode->parent;
			}

			startNode->state = Node::State::Start;
		}

		system("CLS");
		DrawAStar(nodeMap, mapWidth, mapHeight);

		bool inMapMMenu = true;

		//User will not have the option to either toggle obstacles, set new Locations for the start and end nodes, or quit the program.
		//Any action besides quiting the program will run the A* solution again with with whatever change the user made.
		while (inMapMMenu)
		{
			std::cout << "Enter 'start' or 'end' to change the start/end nodes, \n'obstacle' to toggle an obstacle, \nor 'quit' to end the program: ";

			std::string input;
			std::cin >> input;

			for (char& c : input)
			{
				c = tolower(c);
			}

			if (input == "quit")
			{
				inMapMMenu = false;
				solvingAStar = false;
			}
			else if (input == "start")
			{
				inMapMMenu = false;
				bool gettingStartNode = true;

				while (gettingStartNode)
				{
					std::cout << "Enter Starting Node X: ";

					int startX;
					std::cin >> startX;

					if(CheckAndManageCinFail())
					{
						std::cout << "Invalid Input!" << std::endl;
						continue;
					}
					else if (startX < 0 || startX >= mapWidth)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Out of Bounds!" << std::endl;
						continue;
					}

					std::cin.ignore(32767, '\n');
					std::cout << "Enter Starting Node Y: ";

					int startY;
					std::cin >> startY;

					if (CheckAndManageCinFail())
					{
						std::cout << "Invalid Input!" << std::endl;
					}
					else if (startY < 0 || startY >= mapHeight)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Out of Bounds!" << std::endl;
					}
					else if (startX == endNode->GetX() && startY == endNode->GetY())
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Start Node location is the same as End Node, enter new coordinates!" << std::endl;
					}
					else if (nodeMap[(startY * mapWidth) + startX].state == Node::State::Obstacle)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Location is already an obstacle, enter new coordinates!" << std::endl;
					}
					else
					{
						std::cin.ignore(32767, '\n');
						startNode = &nodeMap[(startY * mapWidth) + startX];
						gettingStartNode = false;
					}
				}
			}
			else if (input == "end")
			{
				inMapMMenu = false;
				bool gettingEndNode = true;

				while (gettingEndNode)
				{
					std::cout << "Enter End Node X: ";

					int endX;
					std::cin >> endX;

					if (CheckAndManageCinFail())
					{
						std::cout << "Invalid Input!" << std::endl;
						continue;
					}
					else if (endX < 0 || endX >= mapWidth)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Out of Bounds!" << std::endl;
						continue;
					}

					std::cin.ignore(32767, '\n');
					std::cout << "Enter End Node Y: ";

					int endY;
					std::cin >> endY;

					if (CheckAndManageCinFail())
					{
						std::cout << "Invalid Input!" << std::endl;
					}
					else if (endY < 0 || endY >= mapHeight)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Out of Bounds!" << std::endl;
					}
					else if (endX == startNode->GetX() && endY == startNode->GetY())
					{
						std::cin.ignore(32767, '\n');
						std::cout << "End Node location is the same as Start Node, enter new coordinates!" << std::endl;
					}
					else if (nodeMap[(endY * mapWidth) + endX].state == Node::State::Obstacle)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Location is already an obstacle, enter new coordinates!" << std::endl;
					}
					else
					{
						std::cin.ignore(32767, '\n');
						endNode = &nodeMap[(endY * mapWidth) + endX];
						gettingEndNode = false;
					}
				}
			}
			else if (input == "obstacle")
			{
				inMapMMenu = false;
				bool togglingObstacle = true;

				while (togglingObstacle)
				{
					std::cout << "Enter coordinates to toggle on or off an Obstacle Node" << std::endl;
					std::cout << "Enter Obstacle X: ";

					int obsX;
					std::cin >> obsX;

					if (CheckAndManageCinFail())
					{
						std::cout << "Invalid Input!" << std::endl;
						continue;
					}
					else if (obsX < 0 || obsX >= mapWidth)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Out of Bounds!" << std::endl;
						continue;
					}

					std::cin.ignore(32767, '\n');
					std::cout << "Enter Obstacle Y: ";

					int obsY;
					std::cin >> obsY;

					if (CheckAndManageCinFail())
					{
						std::cout << "Invalid Input!" << std::endl;
					}
					else if (obsY < 0 || obsY >= mapHeight)
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Out of Bounds!" << std::endl;
					}
					else if (obsX == startNode->GetX() && obsY == startNode->GetY())
					{
						std::cin.ignore(32767, '\n');
						std::cout << "Start Node can't become an Obstacle, enter new coordinates!" << std::endl;
					}
					else if (obsX == endNode->GetX() && obsY == endNode->GetY())
					{
						std::cin.ignore(32767, '\n');
						std::cout << "End Node can't become an Obstacle, enter new coordinates!" << std::endl;
					}
					else if (nodeMap[(obsY * mapWidth) + obsX].state == Node::State::Obstacle)
					{
						std::cin.ignore(32767, '\n');
						nodeMap[(obsY * mapWidth) + obsX].state = Node::State::Unvisited;
						togglingObstacle = false;
					}
					else
					{
						std::cin.ignore(32767, '\n');
						nodeMap[(obsY * mapWidth) + obsX].state = Node::State::Obstacle;
						togglingObstacle = false;
					}
				}
			}
			else
			{
				std::cout << "Invalid input, please enter again." << std::endl;
			}
		}
	}
}

int main()
{
	const int minMapSideLength = 2;
	const int maxMapSideLength = 30;
	Node* nodeMap = nullptr;
	bool creatingDiagonalMap = false;
	bool selectingMapType = true;
	std::cout << "Do you want a diangonal path? (Y/N)" << std::endl;

	//User chooses whether to have a diagonal map or a normal non-diagnal (plus) map.
	while(selectingMapType)
	{
		char input;
		input = _getch();
		input = tolower(input);

		if (input == 'y')
		{
			creatingDiagonalMap = true;
			selectingMapType = false;
			std::cout << "Diagonal map selected." << std::endl;
		}
		else if(input == 'n')
		{
			selectingMapType = false;
			std::cout << "Diagonal map not selected." << std::endl;
		}
	}

	int mapWidth;
	int mapHeight;
	bool creatingMap = true;

	//Ask user for map dimensions (must be within the range of the consts minMapeSideLength and maxMapSideLength)
	while (creatingMap)
	{
		std::cout << "Enter Map Width (in range of " << minMapSideLength << " to " << maxMapSideLength << "): ";
		std::cin >> mapWidth;

		if (CheckAndManageCinFail())
		{
			std::cout << "Invalid Input!" << std::endl;
			continue;
		}
		else if(mapWidth < minMapSideLength || mapWidth > maxMapSideLength)
		{
			std::cin.ignore(32767, '\n');
			std::cout << "Invalid Map Width. Please enter side lengths between in the range of " << minMapSideLength << " to " << maxMapSideLength << "." << std::endl;
			continue;
		}

		std::cin.ignore(32767, '\n');
		std::cout << "Enter Map Height (in range of " << minMapSideLength << " to " << maxMapSideLength << "): ";
		std::cin >> mapHeight;

		if (CheckAndManageCinFail())
		{
			std::cout << "Invalid Input!" << std::endl;
		}
		else if (mapHeight < minMapSideLength || mapHeight > maxMapSideLength)
		{
			std::cin.ignore(32767, '\n');
			std::cout << "Invalid Map Height. Please enter side lengths between in the range of " << minMapSideLength << " to " << maxMapSideLength << "." << std::endl;
		}
		else
		{
			std::cin.ignore(32767, '\n');

			if (creatingDiagonalMap) 
			{
				nodeMap = CreateDiagonalNodeMap(mapWidth, mapHeight);
				creatingMap = false;
			}
			else
			{
				nodeMap = CreateNodeMap(mapWidth, mapHeight);
				creatingMap = false;
			}
		}
	}

	SolveAStar(nodeMap, mapWidth, mapHeight);

	//Free up that memory when done like a good programmer should.
	delete[] nodeMap;
	nodeMap = nullptr;

    return 0;
}

