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

	int games_counter = 0;

	if (TRAIN) {
		neat::pool p(127, 9);
		p.import_fromfile("monopoly_level3_ai.res");
		srand(time(NULL));
		unsigned int max_fitness = 0;
		unsigned int second_fitness = 0;
		unsigned int third_fitness = 0;
		unsigned int generations = 100;
		// unsigned int swiss_rounds = 10;
		auto spiece = p.species.begin();

		neat::genome former_best_performer = (*spiece).genomes[0];
		neat::genome best_performer = former_best_performer;

		spiece++;

		neat::genome former_second_performer = (*spiece).genomes[0];
		neat::genome second_performer = former_second_performer;

		spiece++;

		neat::genome former_third_performer = (*spiece).genomes[0];
		neat::genome third_performer = former_third_performer;

		for (int gen = 0; gen < generations; ++gen) {
			max_fitness = 0;
			second_fitness = 0;
			third_fitness = 0;
			for (auto s = p.species.begin(); s != p.species.end(); ++s) {
				// for (int round = 0; round < swiss_rounds; ++round) {
				for (size_t i = 0; i < (*s).genomes.size(); ++i) {
					ann::neuralnet player_1_nn;
					ann::neuralnet player_2_nn;
					ann::neuralnet player_3_nn;
					ann::neuralnet player_4_nn;

					// int first_genome_pos = 4 * i;
					neat::genome& g = (*s).genomes[i];
					// neat::genome& g2 = (*s).genomes[first_genome_pos+1];
					// neat::genome& g3 = (*s).genomes[first_genome_pos+2];
					// neat::genome& g4 = (*s).genomes[first_genome_pos+3];

					player_1_nn.from_genome(g);
					player_2_nn.from_genome(former_best_performer);
					player_3_nn.from_genome(former_second_performer);
					player_4_nn.from_genome(former_third_performer);

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
						++games_counter;
						std::cout << "game counter: " << games_counter
								  << std::endl;
						// printResults(playerResults);

						for (auto player : playerResults) {
							int reward = 5 - player->getResultPlace();
							if (player->getId() == 0) {
								g.fitness += reward;
							}
							// } else if (player->getId() == 1) {
							// 	g2.fitness += reward;
							// } else if (player->getId() == 2) {
							// 	g3.fitness += reward;
							// } else if (player->getId() == 3) {
							// 	g4.fitness += reward;
							// }
						}
					}

					if (g.fitness > max_fitness) {
						third_fitness = second_performer.fitness;
						third_performer = second_performer;
						second_fitness = best_performer.fitness;
						second_performer = best_performer;
						max_fitness = g.fitness;
						best_performer = g;
					} else if (g.fitness > second_fitness) {
						third_fitness = second_performer.fitness;
						third_performer = second_performer;
						second_fitness = g.fitness;
						second_performer = g;
					} else if (g.fitness > third_fitness) {
						third_fitness = g.fitness;
						third_performer = g;
					}
				}
				// std::sort((*s).genomes.begin(), (*s).genomes.end(), genomeComp);
				// }
			}

			former_best_performer = best_performer;
			former_second_performer = second_performer;
			former_third_performer = third_performer;
			p.new_generation();
			if (gen == 15) {
			if (gen == 15) {
				p.export_tofile("monopoly_level1_ai.res");
				ann::neuralnet n;
				n.from_genome(best_performer);
				n.export_tofile("level_1_ai");
			} else if (gen == 29) {
				p.export_tofile("monopoly_level2_ai.res");
				ann::neuralnet n;
				n.from_genome(best_performer);
				n.export_tofile("level_2_ai");
			}
			std::cout << "gen: " << gen << std::endl;
			p.export_tofile("monopoly_level3_ai.res");
			ann::neuralnet n;
			n.from_genome(best_performer);
			n.export_tofile("level_3_ai");
		}
	} else { // play game with all custom players, passed vector is empty
		players.clear();
		playerResults = runMonopolyGame(players);
		printResults(playerResults);
	}

	return 0;
}