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
	monopoly_game_engine_.setPlayerIndexTurn(monopoly_game_engine_.getPlayers()[0].getId());
	monopoly_game_engine_.createButtonRollDice();
	monopoly_game_engine_.createTextTurnInfo();
	monopoly_game_engine_.createTextRolledValue();
	monopoly_game_engine_.createTextPlayersInfo();
	monopoly_game_engine_.createButtonBuyResign();
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

		FieldType field_type = std::visit([](Field& field) { return field.getType(); }, field);
		if (field_type == STREET || field_type == UTILITY || field_type == STATION) {
			float flag_x_pos;
			float flag_y_pos;
			unsigned int field_id = std::visit([](Field& field) { return field.getId(); }, field);
			float field_width = std::visit([](Field& field) { return (float)field.getWidth(); }, field);
			float field_height = std::visit([](Field& field) { return (float)field.getHeight(); }, field);
			float field_rotation = std::visit([](Field& field) { return (float)field.getRotation(); }, field);
			const sf::Vector2i& field_pos = std::visit([](Field& field) { return field.getPosition(); }, field);
			sf::RectangleShape owner_flag = sf::RectangleShape(sf::Vector2f(field_width - 20.0f, field_height * 0.1f));

			// if ((field_id >= 0 && field_id <= 10) ||(field_id > 20 && field_id <= 30)) {
			// 	owner_flag = sf::RectangleShape(sf::Vector2f(field_width - 20.0f, field_height * 0.1f));
			// } else if ((field_id > 10 && field_id <= 20) ||(field_id > 30 && field_id <= 40)) {
			// 	owner_flag = sf::RectangleShape(sf::Vector2f(field_height * 0.1f, field_width - 20.0f));
			// }

			if (field_id >= 0 && field_id <= 10) {
				flag_x_pos = (float)field_pos.x + 10.0f;
				flag_y_pos = (float)field_pos.y + field_height + 8.0f;
			} else if (field_id > 10 && field_id <= 20) {
				flag_x_pos = (float)field_pos.x - field_height - 8.0f;
				flag_y_pos = (float)field_pos.y + 10.0f;
			} else if (field_id > 20 && field_id <= 30) {
				flag_x_pos = (float)field_pos.x - 10.0f;
				flag_y_pos = (float)field_pos.y - field_height - 8.0f;
			} else if (field_id > 30 && field_id <= 40) {
				flag_x_pos = (float)field_pos.x + field_height + 8.0f;
				flag_y_pos = (float)field_pos.y - 10.0f;
			}

			owner_flag.setRotation(field_rotation);
			owner_flag.setOutlineThickness(2.0f);
			owner_flag.setOutlineColor(sf::Color::Black);
			owner_flag.setPosition(sf::Vector2f(flag_x_pos, flag_y_pos));

			switch (field_type) {
				case STREET: {
					StreetField& field_specified = std::get<StreetField>(field);
					std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
					if (owner_ptr != nullptr) {
						owner_flag.setFillColor(owner_ptr->getColor());
					}
					break;
				}

				case UTILITY: {
					UtilityField& field_specified = std::get<UtilityField>(field);
					std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
					if (owner_ptr != nullptr) {
						owner_flag.setFillColor(owner_ptr->getColor());
					}
					break;
				}

				case STATION: {
					StationField& field_specified = std::get<StationField>(field);
					std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
					if (owner_ptr != nullptr) {
						owner_flag.setFillColor(owner_ptr->getColor());
					}
					break;
				}
			}
			getContextWindow()->getWindow().draw(owner_flag);
		}
	}

	for (auto player : monopoly_game_engine_.getPlayers()) {
		getContextWindow()->getWindow().draw(player.getSprite());
	}

	if (monopoly_game_engine_.getTurnState()) {
		getContextWindow()->getWindow().draw(monopoly_game_engine_.getPropertyDataSprite());
		for (auto text_ptr : monopoly_game_engine_.getPropertyDataTexts()) {
			getContextWindow()->getWindow().draw(*text_ptr);
		}
	}
}