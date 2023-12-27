#include "GameScreen.h"

GameScreen::GameScreen(std::vector<std::shared_ptr<playerSettings>> playerSettingsList_){
    std::cout << "GS constructor" << std::endl;
	setScreenType(MonopolyGame);

    // monopoly_game_engine_.setPlayers();

}

GameScreen::~GameScreen(){
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