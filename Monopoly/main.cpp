/**
 * @file main.cpp
 *
 * @brief Source file launching monopoly game vs AI project
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "SFML/Graphics.hpp"
#include "gameEngine.h"

int main() {
	GameEngine gameEngine(30, 1400, 1000);

	gameEngine.worker();

	return 0;
}