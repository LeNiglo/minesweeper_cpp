#include "Game.hpp"

Game::Game(char const *argv[])
{
	this->width = atoi(argv[1]);
	this->width = (width > MAX_X ? MAX_X : (width < MIN_X ? MIN_X : width));
	this->height = atoi(argv[2]);
	this->height = (height > MAX_Y ? MAX_Y : (height < MIN_Y ? MIN_Y : height));
	this->mines = atoi(argv[3]);
	this->mines = (mines > MAX_MINES ? MAX_MINES : (mines < MIN_MINES ? MIN_MINES : mines));
	this->mines = (mines > width * height / 2 ? width * height / 2 - 1 : mines);
	std::cout << "new Game with X: " << this->width << ", Y: " << this->height << ", mines: " << this->mines << std::endl;
	this->window = new sf::RenderWindow(sf::VideoMode(this->width * CELL_SIZE, this->height * CELL_SIZE), "MineSweeper by LeNiglo", 0);
}

Game::~Game()
{
	this->clean();
	delete this->window;
	std::cout << "Thank you for playing !" << std::endl;
}

bool					Game::isRunning()
{
	return this->window->isOpen();
}

void					Game::init()
{
	srand(time(NULL));
	this->board = new Board(this->width, this->height, this->mines);
	this->board->init();
}

void					Game::clean()
{
	if (this->board != NULL) {
		this->board->clean();
		delete this->board;
		this->board = NULL;
	}

}

void					Game::draw()
{
	this->board->draw(this->window);
}

bool					Game::loop()
{
	// TODO, replace by something relevant.
	while (this->isRunning()) {
		this->handleEvents();
		this->window->clear();
		this->draw();
		this->window->display();
	}
	return true;
}

bool					Game::handleEvents()
{
	sf::Event event;
	while (this->window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->window->close();
			return false;
		} else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				this->window->close();
				return false;
			} else if (event.key.code == sf::Keyboard::C && event.key.control) {
				this->window->close();
				return false;
			}
		} else if (event.type == sf::Event::MouseButtonReleased) {
			std::pair<int, int> cell = this->getPosOfCell(event.mouseButton.x, event.mouseButton.y);
			if (event.mouseButton.button == sf::Mouse::Right) {
				std::cout << "Right Clicked on Cell [" << cell.first << ", " << cell.second << "]" << std::endl;
				this->board->markCell(cell);
			} else if (event.mouseButton.button == sf::Mouse::Left) {
				std::cout << "Left Clicked on Cell [" << cell.first << ", " << cell.second << "]" << std::endl;
				this->board->discoverCell(cell);
			}
		}
	}
	return true;
}

std::pair<int, int>		Game::getPosOfCell(const int &posX, const int &posY)
{
	std::pair<int, int> pair;

	pair.first = posX / CELL_SIZE;
	pair.second = posY / CELL_SIZE;

	return pair;
}
