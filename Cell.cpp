#include "Cell.hpp"

//
// Created by Daniel on 10/10/2019.
//
Cell::Cell()
{
	Cell::state = 0;
}
int Cell::status()
{
	return Cell::state;
}
void Cell::update(int state)
{
	Cell::state = state;
}
