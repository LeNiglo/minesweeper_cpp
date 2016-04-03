#ifndef _GAME_HPP_
# define _GAME_HPP_

# include "includes.h"
# include "Board.hpp"

class Game
{
public:
	Game(char const *argv[]);
	~Game();
	bool					isRunning();
	void					init();
	void					clean();
	void					draw();
	bool					loop();
	bool					handleEvents();
	std::pair<int, int>		getPosOfCell(const int &posX, const int &posY);
private:
	sf::RenderWindow		*window;
	int						width;
	int						height;
	int						mines;

	Board					*board;
};

#endif /* end of include guard: _GAME_HPP_ */
