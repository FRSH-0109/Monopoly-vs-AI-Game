#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include "GameScreen.h"
#include "activeScreen.h"
#include "button.h"
#include "contextWindow.h"
#include "main.h"
#include "monopolyGameEngine.h"
#include "typeinfo"
#include "variant"

class DimensionException : public std::exception {
	unsigned int bad_dimension_;

   public:
	DimensionException(unsigned int dimension) : bad_dimension_(dimension){};
	DimensionException(const DimensionException& e) throw() : bad_dimension_(e.bad_dimension_){};
	const unsigned int getBadDimension() { return bad_dimension_; };
};

class RotationException : public std::exception {
	float bad_rotation_;

   public:
	RotationException(float rotation) : bad_rotation_(rotation){};
	RotationException(const RotationException& e) throw() : bad_rotation_(e.bad_rotation_){};
	const float getBadRotation() { return bad_rotation_; };
};

// using PossibleScreens = std::variant<std::unique_ptr<ActiveScreen>, std::unique_ptr<MainMenuScreen>,
// std::unique_ptr<GameMenuScreen>, std::unique_ptr<GameScreen>>;

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
	void worker();

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;

	ContextWindow* getContextWindow();
};

#endif