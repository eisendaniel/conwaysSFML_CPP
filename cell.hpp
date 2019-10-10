//
// Created by Daniel on 10/10/2019.
//

#ifndef _CELL_HPP_
#define _CELL_HPP_

class cell {
 private:
	int state;
 public:
	cell(int state);
	cell();
	int status();
	void update(int state);
};

#endif //_CELL_HPP_
