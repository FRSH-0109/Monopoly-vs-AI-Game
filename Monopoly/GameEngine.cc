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

// =============================================================================
// GameEngine Class Implementation
// =============================================================================

GameEngine::GameEngine(double frame_rate_hz, uint window_width, uint window_height) {
	window_width_ = window_width;
	window_height_ = window_height;

	frame_rate_hz_ = frame_rate_hz;
	frame_rate_delay_ms_ = sf::milliseconds(1000.0 / frame_rate_hz_);

	context_window_ = ContextWindow::GetInstance();
	getContextWindow()->getWindow().create(
		sf::VideoMode(window_width, window_height), "MonopolyVsAI", sf::Style::Default);

	const sf::Vector2i pos(0, 0);
	getContextWindow()->getWindow().setPosition(pos);
	getContextWindow()->getView() = getContextWindow()->getWindow().getDefaultView();

	active_screen_ = std::make_unique<MainMenuScreen>();

	players_.clear();
}

GameEngine::GameEngine(double frame_rate_hz) {
	frame_rate_hz_ = frame_rate_hz;
	frame_rate_delay_ms_ = sf::milliseconds(1000.0 / frame_rate_hz_);

	context_window_ = ContextWindow::GetInstance();
	getContextWindow()->getWindow().create(sf::VideoMode(1, 1), "MonopolyVsAI", sf::Style::Default);

	const sf::Vector2i pos(0, 0);
	getContextWindow()->getWindow().setPosition(pos);
	getContextWindow()->getView() = getContextWindow()->getWindow().getDefaultView();

	// active_screen_ = std::make_unique<MainMenuScreen>();

	players_.clear();
}

ContextWindow* GameEngine::getContextWindow() {
	return context_window_;
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
	if (timeElapsedFromLastFrame_ms >= frame_rate_delay_ms_) {
		clock_frames.restart();
		getContextWindow()->display();
	}
}

std::vector<std::shared_ptr<Player>> GameEngine::worker(std::vector<std::shared_ptr<Player>>& players_vec) {
	players_.clear();
	if (players_vec.size() > 1) {  // if passed more than 1 player to worker
		players_ = players_vec;
		active_screen_.reset();
		active_screen_ = std::make_unique<GameScreen>(players_);
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

		switch (event_type) {
			case PLAY:
				active_screen_.reset();
				active_screen_ = std::make_unique<GameMenuScreen>();
				break;
			case EXIT: {
				players_.clear();
				return players_;  // return empty vector
			} break;
			case RETURN_TO_MAIN_MENU:
				active_screen_.reset();
				active_screen_ = std::make_unique<MainMenuScreen>();
				break;
			case START_GAME: {
				std::vector<std::shared_ptr<playerSettings>> playerSettingsList_;
				playerSettingsList_ = active_screen_->getPlayersSettings();
				players_.clear();
				for (auto playerSettings :
					active_screen_->getPlayersSettings())  // crete temporary players vector to pass to gameScreen and
														  // then monopoly engine
				{
					if (!(playerSettings->isNone)) {
						if (playerSettings->isHuman) {
							Player new_player = Player(0);
							new_player.setIsAi(!(playerSettings->isHuman));
							new_player.setAiLevel(playerSettings->level);
							players_.push_back(std::make_shared<Player>(new_player));
						} else {
							std::string nn_file;
							unsigned int ai_level = playerSettings->level;
							if (ai_level == 1) {
								nn_file = "level_1_ai";
							} else if (ai_level == 2) {
								nn_file = "level_2_ai";
							} else if (ai_level == 3) {
								nn_file = "level_3_ai";
							}
							ann::neuralnet n;
							n.import_fromfile(nn_file);
							srand(time(NULL));
							AiPlayer new_player = AiPlayer(0, n);
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
				active_screen_.reset();
				active_screen_ = std::make_unique<GameScreen>(players_);
			} break;
			case GAME_ENDED:
				return active_screen_->getPlayersResult();
				break;
			default:
				break;
		}

		display();
	}
}

unsigned int GameEngine::getwindow_width() const {
	return window_width_;
}

unsigned int GameEngine::getwindow_height() const {
	return window_height_;
}