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
bool TRAIN = true;
int GAMES_IN_ROUND = 10;

static void printResults(std::vector<std::shared_ptr<Player>>& playerResults) {
	for (int i = playerResults.size() - 1; i >= 0; --i) {
		std::cout << "Player " << playerResults[i]->getId() + 1 << ": miejsce " << playerResults[i]->getResultPlace()
				  << std::endl;
	}
}

static std::vector<std::shared_ptr<Player>> runMonopolyGame(std::vector<std::shared_ptr<Player>>& players) {
	GameEngine gameEngine(FRAMES_PER_SEC_MAX, width, height);
	return gameEngine.worker(players);
}

bool genomeComp(neat::genome g1, neat::genome g2) {
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

	// {	//play game iwth all AI players
	// 	players.clear();
	// 	std::shared_ptr<AiPlayer> player1 = std::make_shared<AiPlayer>(0);
	// 	std::shared_ptr<AiPlayer> player2 = std::make_shared<AiPlayer>(0);
	// 	std::shared_ptr<AiPlayer> player3 = std::make_shared<AiPlayer>(0);
	// 	std::shared_ptr<AiPlayer> player4 = std::make_shared<AiPlayer>(0);
	// 	players.push_back(player1);
	// 	players.push_back(player2);
	// 	players.push_back(player3);
	// 	players.push_back(player4);
	// 	playerResults = runMonopolyGame(players);
	// 	printResults(playerResults);
	// }

	// {	//play game with 1 Human player and 3 AI
	// 	players.clear();
	// 	std::shared_ptr<Player> player1 = std::make_shared<Player>(0);
	// 	std::shared_ptr<AiPlayer> player2 = std::make_shared<AiPlayer>(0);
	// 	std::shared_ptr<AiPlayer> player3 = std::make_shared<AiPlayer>(0);
	// 	std::shared_ptr<AiPlayer> player4 = std::make_shared<AiPlayer>(0);
	// 	players.push_back(player1);
	// 	players.push_back(player2);
	// 	players.push_back(player3);
	// 	players.push_back(player4);
	// 	playerResults = runMonopolyGame(players);
	// 	printResults(playerResults);
	// }

	if (TRAIN) {
		neat::pool p(127, 9);
		p.import_fromfile("monopoly_level1_ai.res");
		srand(time(NULL));
		unsigned int max_fitness = 0;
		unsigned int generations = 100;
		unsigned int swiss_rounds = 10;
		for (int gen = 0; gen < generations; ++gen) {
			for (auto s = p.species.begin(); s != p.species.end(); ++s) {
				for (int round = 0; round < swiss_rounds; ++round) {
					for (size_t i = 0; i < (*s).genomes.size() / 4; ++i) {
						ann::neuralnet player_1_nn;
						ann::neuralnet player_2_nn;
						ann::neuralnet player_3_nn;
						ann::neuralnet player_4_nn;

						int first_genome_pos = 4 * i;

						neat::genome& g1 = (*s).genomes[first_genome_pos];
						neat::genome& g2 = (*s).genomes[first_genome_pos+1];
						neat::genome& g3 = (*s).genomes[first_genome_pos+2];
						neat::genome& g4 = (*s).genomes[first_genome_pos+3];

						player_1_nn.from_genome(g1);
						player_2_nn.from_genome(g2);
						player_3_nn.from_genome(g3);
						player_4_nn.from_genome(g4);

						for (int game = 0; game < GAMES_IN_ROUND; ++game) {
							players.clear();
							std::shared_ptr<AiPlayer> player1 = std::make_shared<AiPlayer>(1500, player_1_nn);
							std::shared_ptr<AiPlayer> player2 = std::make_shared<AiPlayer>(1500, player_2_nn);
							std::shared_ptr<AiPlayer> player3 = std::make_shared<AiPlayer>(1500, player_3_nn);
							std::shared_ptr<AiPlayer> player4 = std::make_shared<AiPlayer>(1500, player_4_nn);
							players.push_back(player1);
							players.push_back(player2);
							players.push_back(player3);
							players.push_back(player4);
							playerResults = runMonopolyGame(players);
							printResults(playerResults);

							for (auto player: playerResults) {
								int reward = 5 - player->getResultPlace();
								if (player->getId() == 0) {
									g1.fitness += reward;
								} else if (player->getId() == 1) {
									g2.fitness += reward;
								} else if (player->getId() == 2) {
									g3.fitness += reward;
								} else if (player->getId() == 3) {
									g4.fitness += reward;
								}
							}
						}
					}
					std::sort((*s).genomes.begin(), (*s).genomes.end(), genomeComp);
				}
			}
			p.new_generation();
			if (gen == 19) {
				p.export_tofile("monopoly_level1_ai.res");
			} else if (gen == 49) {
				p.export_tofile("monopoly_level2_ai.res");
			}
		}
		p.export_tofile("monopoly_level3_ai.res");
	}

	// play game with all custom players, passed vector is empty
	// players.clear();
	// playerResults = runMonopolyGame(players);
	// printResults(playerResults);

	return 0;
}