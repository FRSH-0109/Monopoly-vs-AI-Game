#include "GameScreen.h"
#include <cmath>
#include <numbers>

GameScreen::GameScreen(std::vector<std::shared_ptr<playerSettings>> playerSettingsList_) {
	setContextWindow(ContextWindow::GetInstance());
	std::cout << "GS constructor" << std::endl;
	setScreenType(MonopolyGame);

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	monopoly_game_engine_.setFont(getFont());
	monopoly_game_engine_.clearPlayers();
	monopoly_game_engine_.createPlayers(playerSettingsList_);
	monopoly_game_engine_.createBoard();
	monopoly_game_engine_.setPlayerIndexTurn(0);
	monopoly_game_engine_.createButtonRollDice();
	monopoly_game_engine_.createTextTurnInfo();
	monopoly_game_engine_.createTextRolledValue();
}

GameScreen::~GameScreen() {
	std::cout << "GS destructor" << std::endl;
}

ScreenEventType GameScreen::worker() {
	ScreenEventType eventType = Idle;
	for (auto element : monopoly_game_engine_.getButtons()) {
		if (element->getIsVisible()) {
			if (element->isMouseOver(getContextWindow()->getWindow())) {
				element->mouseIsOver();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					 element->setIsClicked(true);
				}
			} else {
				element->mouseIsNotOver();
			}
			if (element->getIsClicked()) {
				// setOtherButtonsInactive(element);
				// buttonClickHandle(element);
				element->setIsActive(true);
				element->setIsClicked(false);
			}
		}

		buttonSetColors(element);
	}

	monopoly_game_engine_.monopolyGameWorker();

	return eventType;
}

void GameScreen::pollForEvents(sf::Event& event) {}

void GameScreen::draw() {

	std::shared_ptr<Board> game_board_ptr = monopoly_game_engine_.getBoard();

	unsigned int player_position;
	float pos_x;
	float pos_y;
	float x_offset;
	float y_offset;

	unsigned int curr_field_width;
	unsigned int curr_field_height;
	sf::Sprite sprite;

	// draw buttons and texts
	for (auto element : monopoly_game_engine_.getButtons()) {
		if (element->getIsVisible()) {
			element->draw(getContextWindow()->getWindow());
		}
	}
	for (auto element : monopoly_game_engine_.getTexts()) {
		getContextWindow()->getWindow().draw(*element);
	}

	// draw board
	for (auto field : monopoly_game_engine_.getBoard()->getBoard()) {
		sprite = std::visit([](Field& field) { return field.getSprite(); }, field);
		getContextWindow()->getWindow().draw(sprite);
	}

	for (auto player : monopoly_game_engine_.getPlayers()) {
		sprite = player.getSprite();
		player_position = player.getPositon();
		PossibleFields& curr_field = game_board_ptr->getFieldById(player_position);
		curr_field_width = std::visit([](Field& field) { return field.getWidth(); }, curr_field);
		curr_field_height = std::visit([](Field& field) { return field.getHeight(); }, curr_field);
		if (player_position <= 10 ) {
			x_offset = curr_field_width * player.getSpritePositionX();
			y_offset = curr_field_height * player.getSpritePositionY();
		} else if (player_position > 10 && player_position <= 20) {
			x_offset = -curr_field_height * player.getSpritePositionX();
			y_offset = curr_field_width * player.getSpritePositionY();
		} else if (player_position > 20 && player_position <= 30) {
			x_offset = -curr_field_width * player.getSpritePositionX();
			y_offset = -curr_field_height * player.getSpritePositionY();
		} else if (player_position > 30 && player_position <= 40) {
			x_offset = curr_field_height * player.getSpritePositionX();
			y_offset = -curr_field_width * player.getSpritePositionY();
		}
		pos_x = (float)std::visit([](Field& field) { return field.getPosition().x; }, curr_field) + x_offset;
		pos_y = (float)std::visit([](Field& field) { return field.getPosition().y; }, curr_field) + y_offset;
		sprite.setPosition(sf::Vector2f(pos_x, pos_y));
		getContextWindow()->getWindow().draw(sprite);
	}
}