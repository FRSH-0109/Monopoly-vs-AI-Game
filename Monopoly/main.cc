/**
 * @file main.cc
 *
 * @brief Source file launching monopoly game vs AI project
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "SFML/Graphics.hpp"
#include <memory>
#include "Player.h"
#include "GameEngine.h"

unsigned int WIDTH_MAX = 1920;
unsigned int HEIGHT_MAX = 1080;
unsigned int width;
unsigned int height;
unsigned int FRAMES_PER_SEC_MAX = 30;

static void printResults(std::vector<std::shared_ptr<Player>>& playerResults)
{
	for (int i = playerResults.size() - 1; i >= 0; --i) {
		std::cout << "Player " << playerResults[i]->getId() + 1 << ": miejsce " << playerResults[i]->getResultPlace() << std::endl;
	}
}

static std::vector<std::shared_ptr<Player>> runMonopolyGame(std::vector<std::shared_ptr<Player>>& players)
{
	GameEngine gameEngine(FRAMES_PER_SEC_MAX, width, height);
	return gameEngine.worker(players);
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
	std::vector<std::shared_ptr<Player>> players;	//create empty players vector to start game

	{	//play game iwth all AI players
		players.clear();
		std::shared_ptr<AiPlayer> player1 = std::make_shared<AiPlayer>(0);
		std::shared_ptr<AiPlayer> player2 = std::make_shared<AiPlayer>(0);
		std::shared_ptr<AiPlayer> player3 = std::make_shared<AiPlayer>(0);
		std::shared_ptr<AiPlayer> player4 = std::make_shared<AiPlayer>(0);
		players.push_back(player1);
		players.push_back(player2);
		players.push_back(player3);
		players.push_back(player4);
		playerResults = runMonopolyGame(players);
		printResults(playerResults);
	}

	{	//play game with 1 Human player and 3 AI
		players.clear();
		std::shared_ptr<Player> player1 = std::make_shared<Player>(0);
		std::shared_ptr<AiPlayer> player2 = std::make_shared<AiPlayer>(0);
		std::shared_ptr<AiPlayer> player3 = std::make_shared<AiPlayer>(0);
		std::shared_ptr<AiPlayer> player4 = std::make_shared<AiPlayer>(0);
		players.push_back(player1);
		players.push_back(player2);
		players.push_back(player3);
		players.push_back(player4);
		playerResults = runMonopolyGame(players);
		printResults(playerResults);
	}

	//play game with all custom players, passed vector is empty
	players.clear();
	playerResults = runMonopolyGame(players);
	printResults(playerResults);

	return 0;
}