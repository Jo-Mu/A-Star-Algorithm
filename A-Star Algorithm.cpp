// A-Star Algorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Node.h"


int main()
{
	std::cout << "Enter Map Width: ";

	int mapwidth;

	std::cin >> mapwidth;
	std::cout << "Enter Map Height: ";

	int mapheight;

	std::cin >> mapheight;
	std::cout << "Map Width: " << mapwidth << " Map height: " << mapheight << std::endl;	

	Node* nodes = new Node[mapwidth * mapheight];

	std::cout << "Map Size: " << mapwidth * mapheight << std::endl;

    return 0;
}

