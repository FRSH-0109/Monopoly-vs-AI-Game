#include "GameScreen.h"

GameScreen::GameScreen(std::vector<std::shared_ptr<playerSettings>> playerSettingsList_) {
	std::cout << "GS constructor" << std::endl;
	setScreenType(MonopolyGame);

	std::cout << "1: " << playerSettingsList_[0]->isNone << playerSettingsList_[0]->isHuman
			  << playerSettingsList_[0]->level << std::endl;
	std::cout << "1: " << playerSettingsList_[1]->isNone << playerSettingsList_[1]->isHuman
			  << playerSettingsList_[1]->level << std::endl;
	std::cout << "1: " << playerSettingsList_[2]->isNone << playerSettingsList_[2]->isHuman
			  << playerSettingsList_[2]->level << std::endl;
	std::cout << "1: " << playerSettingsList_[3]->isNone << playerSettingsList_[3]->isHuman
			  << playerSettingsList_[3]->level << std::endl;
	// monopoly_game_engine_.setPlayers();
}

GameScreen::~GameScreen() {
	std::cout << "GS destructor" << std::endl;
}

ScreenEventType GameScreen::worker() {
	ScreenEventType eventType = Idle;
	return eventType;
}

void GameScreen::pollForEvents(sf::Event& event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			break;
		case sf::Event::MouseButtonPressed:
			break;
	}
}