#include "Arbiter.hpp"

Arbiter::Arbiter(Board *board)
{
	this->board = board;
}

boost::logic::tribool			Arbiter::check()
{
	int mines = this->board->getMines();
	int flags = this->getFlags(true);
	int freeCells = this->getFreeCells();
	bool foundMines = this->hasFoundMines();

	if (foundMines) {
		// LOOSE CONDITION
		return false;
	}

	if (flags == mines && freeCells == 0) {
		// WIN CONDITION
		return true;
	}
	return boost::logic::indeterminate;
}

int								Arbiter::getFlags(const bool &correct)
{
	int flags = 0;
	std::vector<std::vector<Cell*> > cells = this->board->getCells();

	for (std::size_t i = 0; i < cells.size(); i++) {
		for (std::size_t j = 0; j < cells[i].size(); j++) {
			if (boost::logic::indeterminate(cells[i][j]->getDiscovered()) && (correct ? cells[i][j]->getMine() : true)) {
				++flags;
			}
		}
	}
	return flags;
}

int								Arbiter::getFreeCells()
{
	int freeCells = 0;
	std::vector<std::vector<Cell*> > cells = this->board->getCells();

	for (std::size_t i = 0; i < cells.size(); i++) {
		for (std::size_t j = 0; j < cells[i].size(); j++) {
			if (!cells[i][j]->getDiscovered()) {
				++freeCells;
			}
		}
	}
	return freeCells;
}

bool							Arbiter::hasFoundMines()
{
	std::vector<std::vector<Cell*> > cells = this->board->getCells();

	for (std::size_t i = 0; i < cells.size(); i++) {
		for (std::size_t j = 0; j < cells[i].size(); j++) {
			if (cells[i][j]->getDiscovered() && cells[i][j]->getMine()) {
				return true;
			}
		}
	}
	return false;
}
