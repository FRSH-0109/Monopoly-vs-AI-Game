/**
 * @file gameEngine.cpp
 *
 * @brief Source file for GameEngine class
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "gameEngine.h"

GameEngine::GameEngine(double frameRateHz,
	uint WindowWidth,
	uint WindowHeight) {
	windowWidth_ = WindowWidth;
	windowHeight_ = WindowHeight;

	frameRateHz_ = frameRateHz;
	frameRateDelayMs_ = sf::milliseconds(1000.0 / frameRateHz_);

	contextWindow_ = ContextWindow::GetInstance();
	getContextWindow()->getWindow().create(
		sf::VideoMode(WindowWidth, WindowHeight), "MonopolyVsAI",
		sf::Style::Default);

	const sf::Vector2i pos(0, 0);
	getContextWindow()->getWindow().setPosition(pos);
	getContextWindow()->getView() =
		getContextWindow()->getWindow().getDefaultView();

	setActiveScreen(MainMenu);
}

ContextWindow* GameEngine::getContextWindow() {
	return contextWindow_;
}

void GameEngine::clear() {
	getContextWindow()->getWindow().clear(sf::Color::White);
}

void GameEngine::display() {
	static sf::Clock clock_frames;
	static sf::Time timeElapsedFromLastFrame_ms = sf::milliseconds(0);

	timeElapsedFromLastFrame_ms = clock_frames.getElapsedTime();
	if (timeElapsedFromLastFrame_ms >= frameRateDelayMs_) {
		clock_frames.restart();
		getContextWindow()->display();
	}
}

void GameEngine::pollForEvents(sf::Event& event) {
	switch (event.type) {
		case sf::Event::Closed:
			getContextWindow()->getWindow().close();
			break;
		case sf::Event::Resized:
			// resize my view
			getContextWindow()->getWindow().setSize(
				{event.size.width, event.size.height});
			getContextWindow()->getWindow().setView(
				this->getContextWindow()->getView());
			// and align shape
			break;
	}
}

void GameEngine::worker() {
	menuCreate();

	while (getContextWindow()->isOpen()) {
		clear();

		sf::Event event;
		while (getContextWindow()->getWindow().pollEvent(event)) {
			pollForEvents(event);
			switch (getActiveScreen()) {
				case MainMenu:
					getMenu().pollForEvents(event);
					break;
				case Game:

					break;
			}
		}

		switch (getActiveScreen()) {
			case MainMenu:
				getMenu().draw();
				break;
			case Game:

				break;
		}
		display();
	}
}

void GameEngine::menuCreate() {
	mainMenu_.create();
}

Menu& GameEngine::getMenu() {
	return mainMenu_;
}

uint GameEngine::getWindowWidth() const {
	return windowWidth_;
}

uint GameEngine::getWindowHeight() const {
	return windowHeight_;
}

void GameEngine::setActiveScreen(ContextScreen screen) {
	activeScreen_ = screen;

	switch (activeScreen_) {
		case MainMenu:
			/// screen dependeent actions /*setters
			break;
	}
}

ContextScreen GameEngine::getActiveScreen() const {
	return activeScreen_;
}