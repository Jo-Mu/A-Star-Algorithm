#include "stdafx.h"
#include "Node.h"


Node::Node(int in_x, int in_y)
	:
	x(in_x),
	y(in_y)
{}

int Node::GetX() const
{
	return x;
}

int Node::GetY() const
{
	return y;
}
