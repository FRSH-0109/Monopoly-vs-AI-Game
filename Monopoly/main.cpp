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

	gameEngine.worker();

	return 0;
}