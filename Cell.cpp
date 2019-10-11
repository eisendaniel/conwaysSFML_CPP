#include "Cell.hpp"

//
// Created by Daniel on 10/10/2019.
//
Cell::Cell()
{
	Cell::state = 0;
}
int Cell::getAge()
{
	return Cell::state;
}
void Cell::birth()
{
	Cell::state = 1;
}
void Cell::kill()
{
	Cell::state = 0;
}
void Cell::grow()
{
	if (getAge() >= AGELIMIT) {
		return;
	} else { ++Cell::state; }

}
void Cell::setAge(int state)
{
	Cell::state = state;
}
int Cell::isAlive()
{
	if (getAge()) { return 1; }
	else { return 0; }
}
