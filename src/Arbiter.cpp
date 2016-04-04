#include "Arbiter.hpp"

Arbiter::Arbiter(Board *board)
{
	this->board = board;
}

boost::logic::tribool			Arbiter::check()
{
	int mines = this->board->getMines();
	int flags = 0;
	int freeCells = 0;
	int foundMines = 0;
	std::vector<std::vector<Cell*> > cells = this->board->getCells();

	for (std::size_t i = 0; i < cells.size(); i++) {
		for (std::size_t j = 0; j < cells[i].size(); j++) {
			if (!cells[i][j]->getDiscovered()) {
				++freeCells;
			} else if (boost::logic::indeterminate(cells[i][j]->getDiscovered())) {
				++flags;
			} else if (cells[i][j]->getDiscovered() && cells[i][j]->getMine()) {
				++foundMines;
			}
		}
	}

	if (foundMines > 0) {
		// LOOSE CONDITION
		return false;
	}

	if (flags == mines && freeCells == 0) {
		// WIN CONDITION
		return true;
	}
	return boost::logic::indeterminate;
}
