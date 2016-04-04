#ifndef _GAME_HPP_
# define _GAME_HPP_

# include "includes.h"
# include "Arbiter.hpp"

class Game
{
public:
	Game(char const *argv[]);
	~Game();
	bool						isRunning();
	void						init();
	void						clean();
	void						draw();
	bool						loop();
	bool						handleEvents();
private:
	sf::RenderWindow			*window;
	int							width;
	int							height;
	int							mines;
	boost::logic::tribool		gameStatus;

	Board						*board;
	Arbiter						*arbiter;
};

#endif /* end of include guard: _GAME_HPP_ */
