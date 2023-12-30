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

	monopoly_game_engine_.clearPlayers();
	monopoly_game_engine_.createPlayers(playerSettingsList_);
	monopoly_game_engine_.createBoard();

	createButtonRollDice();
}

GameScreen::~GameScreen() {
	std::cout << "GS destructor" << std::endl;
}

ScreenEventType GameScreen::worker() {
	ScreenEventType eventType = Idle;
	for (auto element : getButtons()) {
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
				return element->getEventType();
			}
		}

		buttonSetColors(element);
	}
	return eventType;
}

void GameScreen::pollForEvents(sf::Event& event) {}

void GameScreen::draw() {
	// const float PI_F = 3.14159265358979f;

	for (auto element : getButtons()) {
		if (element->getIsVisible()) {
			element->draw(getContextWindow()->getWindow());
		}
	}

	std::shared_ptr<Board> game_board_ptr = monopoly_game_engine_.getBoard();

	unsigned int player_position;
	float pos_x;
	float pos_y;
	float x_offset;
	float y_offset;

	unsigned int curr_field_width;
	unsigned int curr_field_height;

	sf::Sprite sprite;

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

void GameScreen::createButtonRollDice() {
	int fontSize = 30;
	sf::Vector2f buttonSize = sf::Vector2f(120, 60);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonRollDice(new Button(Idle, "Roll Dice", buttonSize, fontSize));
	buttonRollDice->setFont(getFont());
	buttonRollDice->setPosition(sf::Vector2f(500, 500));
	buttonRollDice->setActiveBackColor(activeButtonBackColor);
	buttonRollDice->setActiveTextColor(activeButtonTextColor);
	buttonRollDice->setInactiveBackColor(inActiveButtonBackColor);
	buttonRollDice->setInactiveTextColor(inActiveButtonTextColor);
	buttonRollDice->setFocusBackColor(FocusButtonBackColor);
	buttonRollDice->setFocusTextColor(FocusButtonTextColor);
	buttonRollDice->setIsClicked(false);
	buttonRollDice->setIsVisible(true);
	buttonRollDice->setIsActive(false);
	buttonRollDice->setIsFocus(false);
	addButton(buttonRollDice);
}

void GameScreen::setOtherButtonsInactive(std::shared_ptr<Button> buttonPtr) {
	for (auto element : getButtons()) {
		if (element != buttonPtr) {
			element->setIsActive(false);
		}
	}
}