/**
 * @file GameEngine.cc
 *
 * @brief Source file for GameEngine class,
 * used to handle lowes level program operations as
 * input interactions (mouse, keyboard) or display window
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "GameEngine.h"

GameEngine::GameEngine(double frameRateHz, uint WindowWidth, uint WindowHeight) {
	windowWidth_ = WindowWidth;
	windowHeight_ = WindowHeight;

	frameRateHz_ = frameRateHz;
	frameRateDelayMs_ = sf::milliseconds(1000.0 / frameRateHz_);

	contextWindow_ = ContextWindow::GetInstance();
	getContextWindow()->getWindow().create(
		sf::VideoMode(WindowWidth, WindowHeight), "MonopolyVsAI", sf::Style::Default);

	const sf::Vector2i pos(0, 0);
	getContextWindow()->getWindow().setPosition(pos);
	getContextWindow()->getView() = getContextWindow()->getWindow().getDefaultView();

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
			getContextWindow()->getWindow().setSize({event.size.width, event.size.height});
			getContextWindow()->getWindow().setView(this->getContextWindow()->getView());
			// and align shape
			break;
		default:
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

std::vector<std::shared_ptr<Player>> GameEngine::worker(bool AIonly) {
	if (AIonly) {
		std::vector<std::shared_ptr<playerSettings>> playerSettingsList_;

		std::shared_ptr<playerSettings> player1Settings = std::make_shared<playerSettings>();
		player1Settings->isNone = false;
		player1Settings->isHuman = false;
		player1Settings->level = 1;
		playerSettingsList_.push_back(player1Settings);

		std::shared_ptr<playerSettings> player2Settings = std::make_shared<playerSettings>();
		player2Settings->isNone = false;
		player2Settings->isHuman = false;
		player2Settings->level = 1;
		playerSettingsList_.push_back(player2Settings);

		std::shared_ptr<playerSettings> player3Settings = std::make_shared<playerSettings>();
		player3Settings->isNone = false;
		player3Settings->isHuman = false;
		player3Settings->level = 1;
		playerSettingsList_.push_back(player3Settings);

		std::shared_ptr<playerSettings> player4Settings = std::make_shared<playerSettings>();
		player4Settings->isNone = false;
		player4Settings->isHuman = false;
		player4Settings->level = 1;
		playerSettingsList_.push_back(player4Settings);

		activeScreen_.reset();
		activeScreen_ = std::make_unique<GameScreen>(playerSettingsList_);
	}

	while (getContextWindow()->isOpen()) {
		clear();

		sf::Event event;
		while (getContextWindow()->getWindow().pollEvent(event)) {
			pollForEvents(event);
		}

		ScreenEventType eventType = IDLE;
		activeScreen_->draw();
		eventType = activeScreen_->worker();

		switch (eventType) {
			case PLAY:
				activeScreen_.reset();
				activeScreen_ = std::make_unique<GameMenuScreen>();
				break;
			case EXIT:
				getContextWindow()->window_.close();
				break;
			case RETURN_TO_MAIN_MENU:
				activeScreen_.reset();
				activeScreen_ = std::make_unique<MainMenuScreen>();
				break;
			case START_GAME: {
				std::vector<std::shared_ptr<playerSettings>> playerSettingsList_;
				playerSettingsList_ = activeScreen_->getPlayersSettings();
				int counterOfNones = 0;
				for (auto playerSetting : playerSettingsList_)
				{
					if(playerSetting->isNone)
					{
						++counterOfNones;
					}
				}
				if(counterOfNones >= 3)
				{ break;}
				activeScreen_.reset();
				activeScreen_ = std::make_unique<GameScreen>(playerSettingsList_);
			} break;
			case GAME_ENDED:
				return activeScreen_->getPlayersResult();
				break;
			default:
				break;
		}

		display();
	}
}

unsigned int GameEngine::getWindowWidth() const {
	return windowWidth_;
}

unsigned int GameEngine::getWindowHeight() const {
	return windowHeight_;
}