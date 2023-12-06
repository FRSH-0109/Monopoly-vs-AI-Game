#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include "button.h"
#include "contextWindow.h"
#include "main.h"
#include "menuScreen.h"

class GameEngine {
   private:
	ContextWindow* contextWindow_;

	std::unique_ptr<MenuScreen> activeScreen_;

	double frameRateHz_;
	sf::Time frameRateDelayMs_;
	uint windowWidth_;
	uint windowHeight_;

   public:
	GameEngine(double frameRateHz, uint WindowWidth, uint WindowHeight);
	void clear();
	void display();
	void pollForEvents(sf::Event& event);
	void worker();

	uint getWindowWidth() const;
	uint getWindowHeight() const;

	ContextWindow* getContextWindow();
};

#endif