#include "GameScreen.h"

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

void GameScreen::pollForEvents(sf::Event& event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			for (auto element : getButtons()) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->mouseIsOver();
				} else {
					element->mouseIsNotOver();
				}
			}
			break;
		case sf::Event::MouseButtonPressed:
			for (auto element : getButtons()) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->setIsClicked(true);
				}
			}
			break;
	}
}

void GameScreen::draw() {
	for (auto element : getButtons()) {
		if (element->getIsVisible()) {
			element->draw(getContextWindow()->getWindow());
		}
	}

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

void GameScreen::createButtonRollDice()
{
	int fontSize = 30;
	sf::Vector2f buttonSize = sf::Vector2f(120, 60);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192,192,192); //GREY
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