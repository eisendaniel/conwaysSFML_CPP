//
// Created by Daniel on 10/10/2019.
//
#ifndef _CELL_HPP_
#define _CELL_HPP_

#define AGELIMIT 3

class Cell
{
public:
	Cell();

	int get_age() const;
	int is_alive() const;

	void set_age(int state);
	void birth();
	void kill();
	void grow();

private:
	int state;
};

#endif //_CELL_HPP_
