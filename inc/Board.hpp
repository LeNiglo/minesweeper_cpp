#ifndef _BOARD_HPP_
# define _BOARD_HPP_

# include "includes.h"
# include "Cell.hpp"

class Board
{
public:
	Board(const int&, const int&, const int&);
	~Board();
	bool									init();
	void									clean();
	void									draw(sf::RenderWindow*);
	void									revealMines();
	bool									discoverCell(const std::pair<int, int>&);
	bool									markCell(const std::pair<int, int>&);
	bool									testCell(const std::pair<int, int>&);
	int										getMines() const;
	std::vector<std::vector<Cell*> > 		getCells() const;
private:
	void									putMines();
	void									calcNear();
private:
	int										width;
	int										height;
	int										mines;
	std::vector<std::vector<Cell*> > 		cells;
};

#endif /* end of include guard: _BOARD_HPP_ */
