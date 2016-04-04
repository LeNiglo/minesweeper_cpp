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
	std::cout << "New Game with SIZE_X: " << this->width << ", SIZE_Y: " << this->height << ", NB_MINES: " << this->mines << std::endl
	<< "Window size: " << (this->width * CELL_SIZE) << "x" << (HEADER_HEIGHT + this->height * CELL_SIZE) << std::endl;
	this->window = new sf::RenderWindow(sf::VideoMode(this->width * CELL_SIZE, HEADER_HEIGHT + this->height * CELL_SIZE), "MineSweeper by LeNiglo", 0);
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
	this->gameStatus = boost::logic::indeterminate;
	this->board = new Board(this->width, this->height, this->mines);
	this->board->init();
	this->arbiter = new Arbiter(this->board);
	this->window->setFramerateLimit(10);
	this->sfClock.reset();
}

void					Game::clean()
{
	if (this->board != NULL) {
		this->board->clean();
		delete this->board;
		this->board = NULL;
	}

	if (this->arbiter != NULL) {
		delete this->arbiter;
		this->arbiter = NULL;
	}
}

void					Game::draw()
{
	sf::Font font;
	font.loadFromFile("font.otf");

	this->board->draw(this->window);

	sf::Text textTime;
	textTime.setFont(font);
	textTime.setCharacterSize(HEADER_HEIGHT - 10);
	textTime.setString(std::to_string((int) this->sfClock.getElapsedTime().asSeconds()));
	textTime.setColor(sf::Color::Black);
	sf::FloatRect textRect = textTime.getLocalBounds();
	textTime.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	textTime.setPosition(sf::Vector2f(1 + 200 / 2.0f, HEADER_HEIGHT / 2.0f));
	sf::RectangleShape rect1(sf::Vector2f(200, HEADER_HEIGHT - 2));
	rect1.setPosition(sf::Vector2f(1, 1));
	rect1.setFillColor(sf::Color(200, 200, 200));

	this->window->draw(rect1);
	this->window->draw(textTime);

	int minesRemaining = this->mines - this->arbiter->getFlags(false);

	sf::Text textMines;
	textMines.setFont(font);
	textMines.setCharacterSize(HEADER_HEIGHT - 10);
	textMines.setString(std::to_string(minesRemaining));
	textMines.setColor(sf::Color::Black);
	textRect = textMines.getLocalBounds();
	textMines.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	textMines.setPosition(sf::Vector2f(this->width * CELL_SIZE - 201 + 200 / 2.0f, HEADER_HEIGHT / 2.0f));
	sf::RectangleShape rect2(sf::Vector2f(200, HEADER_HEIGHT - 2));
	rect2.setPosition(sf::Vector2f(this->width * CELL_SIZE - 201, 1));
	rect2.setFillColor(sf::Color(200, 200, 200));

	this->window->draw(rect2);
	this->window->draw(textMines);

	if (!boost::logic::indeterminate(this->gameStatus)) {
		sf::Text text;
		text.setColor(sf::Color::Black);
		text.setFont(font);
		text.setCharacterSize(CELL_SIZE * 2);
		if (this->gameStatus) {
			text.setString("VICTORY !");
		} else if (!this->gameStatus) {
			text.setString("YOU LOSE !");
		}
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(sf::Vector2f((this->width * CELL_SIZE) / 2.0f, HEADER_HEIGHT + (this->height * CELL_SIZE) / 2.0f));
		window->draw(text);
	}
}

bool					Game::loop()
{
	this->sfClock.resume();
	while (this->isRunning()) {
		if (!this->handleEvents()) {
			break;
		}
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
			if (event.key.code == sf::Keyboard::Escape || (event.key.code == sf::Keyboard::C && event.key.control)) {
				this->window->close();
				return false;
			} else if (event.key.code == sf::Keyboard::R) {
				// RESET GAME, SO WE DON'T CLOSE THE WINDOW
				return false;
			}
		} else if (event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.y < HEADER_HEIGHT) {

			} else if (boost::logic::indeterminate(this->gameStatus)) {
				std::pair<int, int> cell = Cell::getPosOfCell(event.mouseButton.x, event.mouseButton.y - HEADER_HEIGHT);
				if (event.mouseButton.button == sf::Mouse::Right) {
					// std::cout << "Right Clicked on Cell [" << cell.first << ", " << cell.second << "]" << std::endl;
					this->board->markCell(cell);
				} else if (event.mouseButton.button == sf::Mouse::Left) {
					// std::cout << "Left Clicked on Cell [" << cell.first << ", " << cell.second << "]" << std::endl;
					this->board->discoverCell(cell);
				}
			}
		}

		boost::logic::tribool newGameStatus = this->arbiter->check();
		if (!boost::logic::indeterminate(newGameStatus)) {
			if (boost::logic::indeterminate(this->gameStatus)) {
				if (!newGameStatus) {
					this->board->revealMines();
					std::cout << "YOU LOSE !" << std::endl;
					this->gameStatus = false;
				} else if (newGameStatus) {
					std::cout << "VICTORY !" << std::endl;
					this->gameStatus = true;
				}
				this->sfClock.pause();
			}
		}
	}
	return true;
}
