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

	activeScreen_ = std::make_unique<MainMenuScreen>();
}

ContextWindow* GameEngine::getContextWindow() {
	return contextWindow_;
}

void GameEngine::clear() {
	getContextWindow()->getWindow().clear(sf::Color::White);
}

void GameEngine::pollForEvents(sf::Event& event) {
	switch (event.type) {  // handling general events
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

	switch (activeScreen_->getScreenType()) {	 // handling screen specific events
		case MainMenu:
		case GameMenu:
			activeScreen_->pollForEvents(event);
			break;
		case Game:
			break;
	}
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

void GameEngine::worker() {
	while (getContextWindow()->isOpen()) {
		clear();

		sf::Event event;
		while (getContextWindow()->getWindow().pollEvent(event)) {
			pollForEvents(event);
		}

		ScreenEventType type = Idle;
		switch (activeScreen_->getScreenType()) {
			case MainMenu:
			case GameMenu:
				activeScreen_->draw();
				type = activeScreen_->worker();
				break;
			case Game:

				break;
		}

		display();
	}
}

uint GameEngine::getWindowWidth() const {
	return windowWidth_;
}

uint GameEngine::getWindowHeight() const {
	return windowHeight_;
}