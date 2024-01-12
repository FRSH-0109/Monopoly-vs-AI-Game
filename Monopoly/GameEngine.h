#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include <vector>
#include <typeinfo>
#include <variant>
#include "GameScreen.h"
#include "ActiveScreen.h"
#include "ContextWindow.h"
#include "main.h"

class GameEngine {
   private:
	ContextWindow* contextWindow_;

	std::unique_ptr<ActiveScreen> activeScreen_;

	double frameRateHz_;
	sf::Time frameRateDelayMs_;
	unsigned int windowWidth_;
	unsigned int windowHeight_;

   public:
	GameEngine(double frameRateHz, uint WindowWidth, uint WindowHeight);
	void clear();
	void display();
	void pollForEvents(sf::Event& event);
	std::vector<std::shared_ptr<Player>> worker(bool AIonly);

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;

	ContextWindow* getContextWindow();
};
