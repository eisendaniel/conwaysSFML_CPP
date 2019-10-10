#include "cell.hpp"

//
// Created by Daniel on 10/10/2019.
//
cell::cell()
{
	cell::state = 0;
}
cell::cell(int state)
{
	cell::state = state;
}
int cell::status()
{
	return cell::state;
}
void cell::update(int state)
{
	cell::state = state;
}
