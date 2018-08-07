// A-Star Algorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Node.h"
#include "math.h"

Node* CreateNodeMap(int mapwidth, int mapheight)
{
	Node* nodeMap = new Node[mapwidth * mapheight];

	for(int x = 0; x < mapwidth; x++)
	{
		for(int y = 0; y < mapheight; y++)
		{
			nodeMap[(y * mapheight) + x] = Node(x, y);
		}
	}

	return nodeMap;
}

float DistanceBetweenNodes(const Node& a, const Node& b)
{
	return sqrtf(((a.GetX() - b.GetX())*(a.GetX() - b.GetX())) + ((a.GetY() - b.GetY()) * (a.GetY() - b.GetY())));
}

void SolveAStar(Node* nodeMap)
{
}

void DrawAStar(const Node* nodeMap)
{
}

int main()
{
	std::cout << "Enter Map Width: ";

	int mapwidth;

	std::cin >> mapwidth;
	std::cout << "Enter Map Height: ";

	int mapheight;

	std::cin >> mapheight;
	std::cout << "Map Width: " << mapwidth << " Map height: " << mapheight << std::endl;	

	Node* nodeMap = CreateNodeMap(mapwidth, mapheight);

	std::cout << "Map Size: " << mapwidth * mapheight << std::endl;

	std::cout << "Distance between Node 4 and 32 is " << DistanceBetweenNodes(nodeMap[3], nodeMap[31]) << std::endl;
    return 0;
}

