#include <cmath>
#include <numbers>
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

	const float PI_F = 3.14159265358979f;

	float x_scaled;
	float y_sclaed;
	sf::Vector2u texture_origin;
	sf::Vector2f texture_scale;
	int dir_x = -1;
	int dir_y = 0;
	unsigned int field_id;
	sf::Sprite sprite;
	float rotation;
	unsigned int width;
	unsigned int height;
	sf::Vector2f pos_vect = sf::Vector2f(700, 700);

	for (auto field : monopoly_game_engine_.getBoard()->getBoard()) {
		field_id = std::visit([](Field& field) { return field.getId(); }, field);
		sprite = std::visit([](Field& field) { return field.getSprite(); }, field);
		width = std::visit([](Field& field) { return field.getWidth(); }, field);
		height = std::visit([](Field& field) { return field.getHeight(); }, field);
		rotation = std::visit([](Field& field) { return field.getRotation(); }, field);
		sprite.setOrigin((float)width, 0.f);
		sprite.setPosition(pos_vect);
		sprite.setRotation(rotation);
		getContextWindow()->getWindow().draw(sprite);
		if (field_id % 10 != 0 || field_id == 0) {
			// texture_origin = sprite.getTexture()->getSize();
			// texture_scale = sprite.getScale();
			// x_scaled = round((float)texture_origin.x * texture_scale.x);
			// y_sclaed = round((float)texture_origin.y * texture_scale.y);
			pos_vect.x += dir_x * (float)width;
			pos_vect.y += dir_y * (float)width;
		} else {
			float arg = (PI_F * -90.0f * (field_id / 10) / 180);
			float cos_val = cos(arg);
			float sin_val = sin(arg);
			dir_x = round(-cos_val);
			dir_y = round(-sin_val);
		}

	}
}