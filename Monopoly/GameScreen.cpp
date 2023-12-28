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
	float x_scaled;
	float y_sclaed;
	sf::Vector2u texture_origin;
	sf::Vector2f texture_scale;
	int dir_x = -1;
	int dir_y = 0;
	sf::Vector2f pos_vect = sf::Vector2f(700, 700);
	for (auto field : monopoly_game_engine_.getBoard()->getBoard()) {
		sf::Sprite sprite = std::visit([](Field& field) { return field.getSprite(); }, field);
		sprite.setPosition(pos_vect);
		getContextWindow()->getWindow().draw(sprite);
		texture_origin = sprite.getTexture()->getSize();
		texture_scale = sprite.getScale();
		x_scaled = round((float)texture_origin.x * texture_scale.x);
		y_sclaed = round((float)texture_origin.y * texture_scale.y);
		pos_vect.x += dir_x * x_scaled;
		pos_vect.y += dir_y * y_sclaed;
	}
}