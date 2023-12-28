#include "GameScreen.h"

GameScreen::GameScreen(std::vector<std::shared_ptr<playerSettings>> playerSettingsList_) {
	setContextWindow(ContextWindow::GetInstance());
	std::cout << "GS constructor" << std::endl;
	setScreenType(MonopolyGame);
	monopoly_game_engine_.clearPlayers();
	monopoly_game_engine_.createPlayers(playerSettingsList_);
	monopoly_game_engine_.createBoard();
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

void GameScreen::draw() {
	for (auto field : monopoly_game_engine_.getBoard()->getBoard()) {
		unsigned int id =  std::visit([](Field& field) { return field.getId(); }, field);
		if(id == 30) {
		sf::Sprite sprite = std::visit([](Field& field) { return field.getSprite(); }, field);
		getContextWindow()->getWindow().draw(sprite);
		}
	}
}