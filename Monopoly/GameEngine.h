/**
 * @file GameEngine.h
 *
 * @brief Header file for GameEngine class,
 * used to handle lowes level program operations as
 * input interactions (mouse, keyboard) or display window
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include <typeinfo>
#include <variant>
#include <vector>
#include "ActiveScreen.h"
#include "ContextWindow.h"
#include "GameScreen.h"
#include "Player.h"
#include "main.h"

class GameEngine {
   private:
	ContextWindow* contextWindow_;

	std::unique_ptr<ActiveScreen> activeScreen_;

	double frameRateHz_;
	sf::Time frameRateDelayMs_;
	unsigned int windowWidth_;
	unsigned int windowHeight_;

	std::vector<std::shared_ptr<Player>> players_;

   public:
	GameEngine(double frameRateHz, uint WindowWidth, uint WindowHeight);
	void clear();
	void display();
	void pollForEvents(sf::Event& event);
	std::vector<std::shared_ptr<Player>> worker(std::vector<std::shared_ptr<Player>>& playersVec);

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;

	ContextWindow* getContextWindow();
};

#endif