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
	sf::Texture textureMine;
	sf::Texture textureFlag;
	font.loadFromFile("font.otf");
	textureMine.loadFromFile("mine.png");
	textureFlag.loadFromFile("flag.png");
	textureMine.setSmooth(true);
	textureFlag.setSmooth(true);
	textureMine.setRepeated(false);
	textureFlag.setRepeated(false);

	// Check if it needed to hover cells
	std::vector<std::pair<int, int> > cellsToHover;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

		if (mousePos.x >= 0 && mousePos.x < this->width * CELL_SIZE && mousePos.y >= HEADER_HEIGHT && mousePos.y < HEADER_HEIGHT + this->height * CELL_SIZE) {
			std::pair<int, int> cell = Cell::getPosOfCell(mousePos.x, mousePos.y - HEADER_HEIGHT);
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					std::pair<int, int> newCell(cell.first + i, cell.second + j);
					cellsToHover.push_back(newCell);
				}
			}
		}
	}

	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
			sf::Sprite spriteMine;
			sf::Sprite spriteFlag;
			sf::Text text;

			bool hoveredCell = false;
			for (size_t inc = 0; inc < cellsToHover.size(); inc++) {
				if (cellsToHover[inc].first == i && cellsToHover[inc].second == j) {
					hoveredCell = true;
					break;
				}
			}

			spriteMine.setTexture(textureMine);
			spriteFlag.setTexture(textureFlag);

			rectangle.setPosition(i * CELL_SIZE, HEADER_HEIGHT + j * CELL_SIZE);
			rectangle.setOutlineThickness(2);

			if (hoveredCell && !this->cells[i][j]->getDiscovered()) {
				rectangle.setFillColor(sf::Color(160, 160, 160));
				rectangle.setOutlineColor(sf::Color(20, 20, 20));
			} else {
				rectangle.setFillColor((i + j) % 2 ? sf::Color(250, 250, 250) : sf::Color(200, 200, 200));
				rectangle.setOutlineColor((i + j) % 2 ? sf::Color(180, 180, 180) : sf::Color(250, 250, 250));
			}

			if (this->cells[i][j]->getDiscovered()) {
				if (this->cells[i][j]->getValue() > 0) {
					if (this->cells[i][j]->getMine()) {
						spriteMine.setPosition(sf::Vector2f(i * CELL_SIZE, HEADER_HEIGHT + j * CELL_SIZE));
					} else {
						int colorShade = (hoveredCell ? 0 : 30) + 100 / this->cells[i][j]->getValue();
						text.setFont(font);
						text.setFillColor(sf::Color(colorShade, colorShade, colorShade));
						text.setPosition(i * CELL_SIZE + CELL_SIZE / 4, HEADER_HEIGHT + j * CELL_SIZE + CELL_SIZE / 4);
						text.setCharacterSize(CELL_SIZE / 2);
						text.setString(std::to_string(this->cells[i][j]->getValue()));
					}
				} else {
					rectangle.setFillColor(sf::Color(120, 120, 120));
				}
			} else if (boost::logic::indeterminate(this->cells[i][j]->getDiscovered())) {
				spriteFlag.setPosition(sf::Vector2f(i * CELL_SIZE, HEADER_HEIGHT + j * CELL_SIZE));
			}

			window->draw(rectangle);
			if (spriteMine.getPosition().y != 0) {
				window->draw(spriteMine);
			}
			if (spriteFlag.getPosition().y != 0) {
				window->draw(spriteFlag);
			}
			if (text.getPosition().y != 0) {
				window->draw(text);
			}
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
