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
	monopoly_game_engine_.createBoard();
	monopoly_game_engine_.clearPlayers();
	monopoly_game_engine_.createPlayers(playerSettingsList_);
	monopoly_game_engine_.setPlayerIndexTurn(0);
	monopoly_game_engine_.createButtonRollDice();
	monopoly_game_engine_.createTextTurnInfo();
	monopoly_game_engine_.createTextRolledValue();
	monopoly_game_engine_.createTextPlayersInfo();
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
	sf::Sprite sprite;
	for (auto field : monopoly_game_engine_.getBoard()->getBoard()) {
		sprite = std::visit([](Field& field) { return field.getSprite(); }, field);
		getContextWindow()->getWindow().draw(sprite);

		// FieldType field_type = std::visit([](Field& field) { return field.getType(); }, field);
		// if (field_type == PROPERTY || field_type == UTILITY || field_type == STATION) {
		// 	sf::RectangleShape owner_flag;
		// 	unsigned int field_id = std::visit([](Field& field) { return field.getId(); }, field);
		// 	float field_width = std::visit([](Field& field) { return (float)field.getWidth(); }, field);
		// 	float field_height = std::visit([](Field& field) { return (float)field.getHeight(); }, field);
		// 	if ((field_id >= 0 && field_id <= 10) ||(field_id > 20 && field_id <= 30)) {
		// 		owner_flag = sf::RectangleShape(sf::Vector2f(field_width - 20.0f, field_height * 0.1f));
		// 	} else if ((field_id > 10 && field_id <= 20) ||(field_id > 30 && field_id <= 40)) {
		// 		owner_flag = sf::RectangleShape(sf::Vector2f(field_height * 0.1f, field_width - 20.0f));
		// 	}
		// 	std::visit([](PropertyField& field) { return field.getOwner()->getColor(); }, field);
		// }
	}

	for (auto player : monopoly_game_engine_.getPlayers()) {
		getContextWindow()->getWindow().draw(player.getSprite());
	}
}