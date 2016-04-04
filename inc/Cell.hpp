#ifndef _CELL_HPP
# define _CELL_HPP

# include "includes.h"
# include <boost/logic/tribool.hpp>

class Cell
{
public:
	Cell(const int&, const int&);
	void					setDiscovered(const boost::logic::tribool&);
	void					setValue(const int&);
	void					setMine(const bool&);
	boost::logic::tribool	getDiscovered() const;
	int						getValue() const;
	bool					getMine() const;
	int						getX() const;
	int						getY() const;
private:
	int 					x;
	int						y;
	int						value;
	boost::logic::tribool	discovered;
	bool					mine;
};

#endif /* end of include guard: _CELL_HPP */
