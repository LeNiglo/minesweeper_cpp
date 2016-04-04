#include "Cell.hpp"

Cell::Cell(const int &x, const int &y)
{
	(void) x;
	(void) y;
	// std::cout << "New cell at [" << x << ", " << y << "]" << std::endl;
	this->mine = false;
	this->value = 0;
	this->discovered = false;
}

void					Cell::setDiscovered(const boost::logic::tribool &status)
{
	this->discovered = status;
}

void					Cell::setValue(const int &v)
{
	this->value = v;
}

void					Cell::setMine(const bool &v)
{
	this->mine = v;
}

boost::logic::tribool	Cell::getDiscovered() const
{
	return this->discovered;
}

int						Cell::getValue() const
{
	return this->value;
}

bool					Cell::getMine() const
{
	return this->mine;
}

int						Cell::getX() const
{
	return this->x;
}

int						Cell::getY() const
{
	return this->y;
}

std::pair<int, int>		Cell::getPosOfCell(const int &posX, const int &posY)
{
	std::pair<int, int> pair;

	pair.first = posX / CELL_SIZE;
	pair.second = posY / CELL_SIZE;

	return pair;
}
