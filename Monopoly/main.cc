/**
 * @file main.cc
 *
 * @brief Source file launching monopoly game vs AI project
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "SFML/Graphics.hpp"
#include "GameEngine.h"

bool AIonly = false;

int main() {
	unsigned int WIDTH_MAX = 1920;
	unsigned int HEIGHT_MAX = 1080;
	unsigned int width = sf::VideoMode::getDesktopMode().width;
	unsigned int height = sf::VideoMode::getDesktopMode().height;
	if (width > WIDTH_MAX) {
		width = WIDTH_MAX;
	}

	if (height > HEIGHT_MAX) {
		height = HEIGHT_MAX;
	}
	GameEngine gameEngine(30, width, height);
	std::vector<std::shared_ptr<Player>> playerResults = gameEngine.worker(AIonly);

	for (int i = playerResults.size() - 1; i >= 0; --i) {
		std::cout << "Player " << playerResults[i]->getId() + 1 << ": miejsce " << playerResults[i]->getResultPlace() << std::endl;
	}

	// gameEngine = GameEngine(30, width, height);
	// gameEngine.worker(AIonly);

	// gameEngine = GameEngine(30, width, height);
	// gameEngine.worker(AIonly);

	return 0;
}