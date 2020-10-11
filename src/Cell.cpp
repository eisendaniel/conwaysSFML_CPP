#include "Cell.hpp"

//
// Created by Daniel on 10/10/2019.
//
Cell::Cell()
{
	Cell::state = 0;
}
int Cell::get_age()
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
	if (get_age() >= AGELIMIT)
		return;
	else
		++Cell::state;
}
void Cell::set_age(int state)
{
	Cell::state = state;
}
int Cell::is_alive()
{
	if (get_age() > 0)
		return 1;
	else
		return 0;
}
