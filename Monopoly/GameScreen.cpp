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
	monopoly_game_engine_.createButtonBuyResign();
	monopoly_game_engine_.createButtonNextProperty();
	monopoly_game_engine_.createButtonPerviousProperty();
	monopoly_game_engine_.showPropertyData(1, false);
	monopoly_game_engine_.createButtonsBuySellHouseHotel();
	monopoly_game_engine_.createButtonsBankrupt();
	monopoly_game_engine_.createButtonsNextTurn();
}

GameScreen::~GameScreen() {
	std::cout << "GS destructor" << std::endl;
}

ScreenEventType GameScreen::worker() {
	ScreenEventType eventType = Idle;
	for (auto element : monopoly_game_engine_.getButtons()) {
		element->setIsClicked(false);
		element->setIsActive(false);
		if (element->getIsVisible()) {
			if (element->isMouseOver(getContextWindow()->getWindow())) {
				element->mouseIsOver();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					element->setIsClicked(true);
					element->setIsActive(true);
				} else {
					element->setWasReleased(true);
				}
			} else {
				element->setWasReleased(true);
				element->mouseIsNotOver();
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
		FieldType field_type = std::visit([](Field& field) { return field.getType(); }, field);
		sprite = std::visit([](Field& field) { return field.getSprite(); }, field);
		getContextWindow()->getWindow().draw(sprite);
		float rotation = std::visit([](Field& field) { return field.getRotation(); }, field);

		// draw property name text
		sf::Text nameText;
		if (field_type == STREET || field_type == UTILITY || field_type == STATION || field_type == TAX) {
			nameText = std::visit([](Field& field) { return field.getNameText(); }, field);
			nameText = monopoly_game_engine_.getPropertyNameToDraw(nameText, sprite, rotation);
			getContextWindow()->getWindow().draw(nameText);
		}

		if (field_type == STREET || field_type == UTILITY || field_type == STATION) {
			sf::RectangleShape owner_flag;
			switch (field_type) {
				case STREET: {
					StreetField& field_specified = std::get<StreetField>(field);
					owner_flag = field_specified.getOwnerFlag();
					std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
					if (owner_ptr != nullptr) {
						owner_flag.setFillColor(owner_ptr->getColor());

						if (field_specified.getHouseNumber() != 0) {
							for (unsigned int i = 1; i <= field_specified.getHouseNumber(); ++i) {
								sf::Sprite house_sprite = monopoly_game_engine_.getHouseSprite(field_specified, i);
								sf::Texture house_texture = monopoly_game_engine_.getHouseTexture();
								house_sprite.setTexture(house_texture, true);
								getContextWindow()->getWindow().draw(house_sprite);
							}
						} else if (field_specified.getIsHotel()) {
							sf::Sprite hotel_sprite = monopoly_game_engine_.getHotelSprite(field_specified);
							sf::Texture hotel_texture = monopoly_game_engine_.getHotelTexture();
							hotel_sprite.setTexture(hotel_texture, true);
							getContextWindow()->getWindow().draw(hotel_sprite);
						}
					}
				} break;

				case UTILITY: {
					UtilityField& field_specified = std::get<UtilityField>(field);
					owner_flag = field_specified.getOwnerFlag();
					std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
					if (owner_ptr != nullptr) {
						owner_flag.setFillColor(owner_ptr->getColor());
					}
				} break;

				case STATION: {
					StationField& field_specified = std::get<StationField>(field);
					owner_flag = field_specified.getOwnerFlag();
					std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
					if (owner_ptr != nullptr) {
						owner_flag.setFillColor(owner_ptr->getColor());
					}
				} break;
			}

			getContextWindow()->getWindow().draw(owner_flag);
		}
	}

	// draw player
	for (auto player : monopoly_game_engine_.getPlayers()) {
		getContextWindow()->getWindow().draw(player->getSprite());
	}

	// draw property data for buy/sell
	if (monopoly_game_engine_.getTurnState() == BuyAction) {
		getContextWindow()->getWindow().draw(monopoly_game_engine_.getPropertyDataSprite());
		for (auto text_ptr : monopoly_game_engine_.getPropertyDataTexts()) {
			getContextWindow()->getWindow().draw(*text_ptr);
		}
	}

	// draw property browser
	getContextWindow()->getWindow().draw(monopoly_game_engine_.getAllPropertyDataSprite());
	for (auto text_ptr : monopoly_game_engine_.getAllPropertyDataTexts()) {
		getContextWindow()->getWindow().draw(*text_ptr);
	}

	// Notifications wall draw
	for (auto notiTextPtr : monopoly_game_engine_.getNotificationsWall().getWall()) {
		getContextWindow()->getWindow().draw(*notiTextPtr);
	}
}
