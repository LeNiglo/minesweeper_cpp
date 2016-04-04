#include "Board.hpp"

Board::Board(const int &x, const int &y, const int &m)
{
	this->width = x;
	this->height = y;
	this->mines = m;
	for (int i = 0; i < x; i++) {
		std::vector<Cell*> v(y, NULL);
		this->cells.push_back(v);
	}
}

Board::~Board()
{
	this->clean();
}

bool									Board::init()
{
	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			this->cells[i][j] = new Cell(i, j);
		}
	}

	this->putMines();
	this->calcNear();
	return true;
}

void									Board::clean()
{
	if (!this->cells.empty()) {
		for (int i = 0; i < this->width; i++) {
			for (int j = 0; j < this->height; j++) {
				if (!this->cells[i].empty() && this->cells[i][j] != NULL) {
					delete this->cells[i][j];
				}
			}
		}
		this->cells.clear();
	}
}

void								Board::draw(sf::RenderWindow *window)
{
	sf::Font font;
	font.loadFromFile("font.otf");

	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
			sf::Text text;
			rectangle.setPosition(i * CELL_SIZE, HEADER_HEIGHT + j * CELL_SIZE);
			rectangle.setFillColor((i + j) % 2 ? sf::Color(250, 250, 250) : sf::Color(180, 180, 180));
			rectangle.setOutlineThickness(2);
			rectangle.setOutlineColor((i + j) % 2 ? sf::Color(180, 180, 180) : sf::Color(250, 250, 250));
			window->draw(rectangle);

			text.setFont(font);
			text.setPosition(i * CELL_SIZE + CELL_SIZE / 4, HEADER_HEIGHT + j * CELL_SIZE + CELL_SIZE / 4);
			text.setCharacterSize(CELL_SIZE / 2);

			if (this->cells[i][j]->getDiscovered()) {
				if (this->cells[i][j]->getValue() > 0) {
					text.setColor(sf::Color(150 / this->cells[i][j]->getValue(), 150 / this->cells[i][j]->getValue(), 150 / this->cells[i][j]->getValue()));

					if (this->cells[i][j]->getMine()) {
						text.setString("X");
						text.setStyle(sf::Text::Bold);
						text.setColor(sf::Color::Red);
					} else {
						text.setString(std::to_string(this->cells[i][j]->getValue()));
					}

				} else {
					rectangle.setFillColor(sf::Color(120, 120, 120));
				}
			} else if (boost::logic::indeterminate(this->cells[i][j]->getDiscovered())) {
				text.setString("F");
				text.setColor(sf::Color::Blue);
			}

			window->draw(rectangle);
			window->draw(text);
		}
	}
}

void								Board::revealMines()
{
	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			if (this->cells[i][j]->getMine() && (this->cells[i][j]->getDiscovered() || !this->cells[i][j]->getDiscovered())) {
				this->cells[i][j]->setDiscovered(true);
			}
		}
	}
}

bool								Board::discoverCell(const std::pair<int, int> &cell)
{
	if (cell.first >= 0 && cell.first < this->width && cell.second >= 0 && cell.second < this->height) {
		if (!this->cells[cell.first][cell.second]->getDiscovered()) {
			this->cells[cell.first][cell.second]->setDiscovered(true);

			if (this->cells[cell.first][cell.second]->getMine()) {
				std::cout << "BOOOOOOOOM !" << std::endl;
			}

			if (this->cells[cell.first][cell.second]->getValue() == 0) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						std::pair<int, int> newCell(cell.first + i, cell.second + j);
						this->discoverCell(newCell);
					}
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}


bool								Board::markCell(const std::pair<int, int> &cell)
{
	if (cell.first >= 0 && cell.first < this->width && cell.second >= 0 && cell.second < this->height) {
		if (!this->cells[cell.first][cell.second]->getDiscovered()) {
			this->cells[cell.first][cell.second]->setDiscovered(boost::logic::indeterminate);
			return true;
		} else if (boost::logic::indeterminate(this->cells[cell.first][cell.second]->getDiscovered())) {
			this->cells[cell.first][cell.second]->setDiscovered(false);
			return true;
		} else {
			return this->testCell(cell);
		}
	} else {
		return false;
	}
}

bool								Board::testCell(const std::pair<int, int> &cell)
{
	if (cell.first >= 0 && cell.first < this->width && cell.second >= 0 && cell.second < this->height) {
		if (this->cells[cell.first][cell.second]->getDiscovered()) {

			int flagsNear = 0;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (cell.first + i >= 0 && cell.first + i < this->width && cell.second + j >= 0 && cell.second + j < this->height) {
						if (boost::logic::indeterminate(this->cells[cell.first + i][cell.second + j]->getDiscovered())) {
							++flagsNear;
						}
					}
				}
			}

			if (flagsNear == this->cells[cell.first][cell.second]->getValue()) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						std::pair<int, int> newCell(cell.first + i, cell.second + j);
						this->discoverCell(newCell);
					}
				}
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

int									Board::getMines() const
{
	return this->mines;
}

std::vector<std::vector<Cell*> > 	Board::getCells() const
{
	return this->cells;
}

void								Board::putMines()
{
	int putMines = 0;
	while (putMines < this->mines) {
		std::pair<int, int> pos;

		pos.first = rand() % this->width;
		pos.second = rand() % this->height;

		if (this->cells[pos.first][pos.second]->getMine() == false) {
			// std::cout << "Putting mine at [" << pos.first << ", " << pos.second << "]" << std::endl;
			this->cells[pos.first][pos.second]->setMine(true);
			++putMines;
		}
	}
}

void								Board::calcNear()
{
	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			int nears = 0;

			for (int relativeX = -1; relativeX <= 1; relativeX++) {
				for (int relativeY = -1; relativeY <= 1; relativeY++) {
					if (i + relativeX >= 0 && i + relativeX < this->width && j + relativeY >= 0 && j + relativeY < this->height) {
						if (this->cells[i + relativeX][j + relativeY]->getMine() == true) {
							++nears;
						}
					}
				}
			}

			this->cells[i][j]->setValue(nears);
		}
	}
}
