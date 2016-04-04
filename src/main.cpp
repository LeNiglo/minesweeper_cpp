#include "Game.hpp"

int main(int argc, char const *argv[])
{
	if (argc < 4) {
		std::cerr << "USAGE: " << std::endl << argv[0] << " SIZE_X SIZE_Y NB_MINES" << std::endl;
		return EXIT_FAILURE;
	}

	Game g(argv);
	bool status = true;
	while (g.isRunning() && status) {
		g.init();
		status = g.loop();
		g.clean();
	}

	return status ? EXIT_SUCCESS : EXIT_FAILURE;
}
