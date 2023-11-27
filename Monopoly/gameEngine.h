#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "button.h"
#include "contextWindow.h"
#include "mainMenu.h"

enum ContextScreen { None, MainMenu, GameMenu, Game };

class GameEngine {
   private:
	ContextWindow* contextWindow_;

	ContextScreen activeScreen_;

	Menu mainMenu_;

	double frameRateHz_;
	sf::Time frameRateDelayMs_;
	uint windowWidth_;
	uint windowHeight_;

   public:
	GameEngine(double frameRateHz, uint WindowWidth, uint WindowHeight);
	void clear();
	void display();
	void pollForEvents(sf::Event& event);
	void draw();
	void worker();

	void menuCreate();
	Menu& getMenu();

	uint getWindowWidth() const;
	uint getWindowHeight() const;

	void setActiveScreen(ContextScreen screen);
	ContextScreen getActiveScreen() const;

	ContextWindow* getContextWindow();
};

#endif