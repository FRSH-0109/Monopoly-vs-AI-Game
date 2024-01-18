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

	players_.clear();
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

std::vector<std::shared_ptr<Player>> GameEngine::worker(std::vector<std::shared_ptr<Player>>& playersVec) {
	players_.clear();
	if (playersVec.size() > 1) {  // if passed more than 1 player to worker
		players_ = playersVec;
		activeScreen_.reset();
		activeScreen_ = std::make_unique<GameScreen>(players_);
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
			case EXIT: {
				players_.clear();
				return players_;  // return empty vector
			} break;
			case RETURN_TO_MAIN_MENU:
				activeScreen_.reset();
				activeScreen_ = std::make_unique<MainMenuScreen>();
				break;
			case START_GAME: {
				std::vector<std::shared_ptr<playerSettings>> playerSettingsList_;
				playerSettingsList_ = activeScreen_->getPlayersSettings();
				players_.clear();
				for (auto playerSettings :
					activeScreen_->getPlayersSettings())  // crete temporary players vector to pass to gameScreen and
														  // then monopoly engine
				{
					if (!(playerSettings->isNone)) {
						if (playerSettings->isHuman) {
							Player new_player = Player(0);
							new_player.setIsAi(!(playerSettings->isHuman));
							new_player.setAiLevel(playerSettings->level);
							players_.push_back(std::make_shared<Player>(new_player));
						} else {
							AiPlayer new_player = AiPlayer(0);
							new_player.setIsAi(!(playerSettings->isHuman));
							new_player.setAiLevel(playerSettings->level);
							players_.push_back(std::make_shared<AiPlayer>(new_player));
						}
					}
				}
				int counterOfNones = 0;
				for (auto playerSetting : playerSettingsList_)	// block game with 1 player
				{
					if (playerSetting->isNone) {
						++counterOfNones;
					}
				}
				if (counterOfNones >= 3) {
					break;
				}
				activeScreen_.reset();
				activeScreen_ = std::make_unique<GameScreen>(players_);
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