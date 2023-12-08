#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include "activeScreen.h"
#include "button.h"
#include "contextWindow.h"
#include "main.h"
#include "monopolyGameEngine.h"

class GameEngine {
   private:
	ContextWindow* contextWindow_;

	std::unique_ptr<ActiveScreen> activeScreen_;

	double frameRateHz_;
	sf::Time frameRateDelayMs_;
	unsigned int windowWidth_;
	unsigned int windowHeight_;

	monopolyGameEngine monopolyEngine;

   public:
	GameEngine(double frameRateHz, uint WindowWidth, uint WindowHeight);
	void clear();
	void display();
	void pollForEvents(sf::Event& event);
	void worker();

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;

	ContextWindow* getContextWindow();
};

#endif