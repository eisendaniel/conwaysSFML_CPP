//
// Created by Daniel on 10/10/2019.
//
#ifndef _CELL_HPP_
#define _CELL_HPP_

#define AGELIMIT 3

class Cell {
 private:
	int state;
 public:
	Cell();
	int getAge();
	int isAlive();
	void setAge(int state);
	void birth();
	void kill();
	void grow();
};

#endif //_CELL_HPP_
