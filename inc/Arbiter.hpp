#ifndef _ARBITER_HPP
# define _ARBITER_HPP

# include "includes.h"
# include "Board.hpp"

class Arbiter {
public:
	Arbiter(Board*);
	boost::logic::tribool		check();
	int							getFlags(const bool&);
	int							getFreeCells();
	bool						hasFoundMines();
private:
	Board						*board;
};

#endif /* end of include guard: _ARBITER_HPP */
