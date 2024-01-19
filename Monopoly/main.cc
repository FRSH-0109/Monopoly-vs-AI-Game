/**
 * @file main.cc
 *
 * @brief Source file launching monopoly game vs AI project
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include <memory>
#include "GameEngine.h"
#include "Player.h"
#include "SFML/Graphics.hpp"

unsigned int WIDTH_MAX = 1920;
unsigned int HEIGHT_MAX = 1080;
unsigned int width;
unsigned int height;

unsigned int FRAMES_PER_SEC_MAX = 30;

bool TRAIN = false;
int GAMES_IN_ROUND = 5;

static void printResults(std::vector<std::shared_ptr<Player>>& playerResults) {
	for (int i = playerResults.size() - 1; i >= 0; --i) {
		std::cout << "Player " << playerResults[i]->getId() + 1 << ": miejsce " << playerResults[i]->getResultPlace()
				  << std::endl;
	}
}

static std::vector<std::shared_ptr<Player>> runMonopolyGame(std::vector<std::shared_ptr<Player>> players) {
	std::unique_ptr<GameEngine> gameEngine = std::make_unique<GameEngine>(FRAMES_PER_SEC_MAX, width, height); // , width, height
	std::vector<std::shared_ptr<Player>> players_ret = gameEngine->worker(players);
	gameEngine->getContextWindow()->getWindow().close();
	gameEngine.reset();
	return players_ret;
}

static bool genomeComp(neat::genome g1, neat::genome g2) {
	return g1.fitness > g2.fitness;
}

int main() {
	width = sf::VideoMode::getDesktopMode().width;
	height = sf::VideoMode::getDesktopMode().height;
	if (width > WIDTH_MAX) {
		width = WIDTH_MAX;
	}
	if (height > HEIGHT_MAX) {
		height = HEIGHT_MAX;
	}

	std::vector<std::shared_ptr<Player>> playerResults;
	std::vector<std::shared_ptr<Player>> players;  // create empty players vector to start game
	playerResults = runMonopolyGame(players);
	printResults(playerResults);

	return 0;
}