/**
 * @file ActiveScreen.cc
 *
 * @brief Source file handling displayed screens of  project
 * Base claass is ActiveScreen, then derived class are used to
 * work with specific screen shown.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "ActiveScreen.h"

ActiveScreen::ActiveScreen() {}

void ActiveScreen::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& ActiveScreen::getFont() {
	return font_;
}

void ActiveScreen::addButton(std::shared_ptr<Button> buttonTmp) {
	std::shared_ptr<Button> button = buttonTmp;
	buttons_.push_back(button);
}

void ActiveScreen::addText(std::shared_ptr<sf::Text> textTmp) {
	std::shared_ptr<sf::Text> text = textTmp;
	texts_.push_back(text);
}

void MainMenuScreen::draw() {
	for (auto element : getButtons()) {
		if (element->getIsVisible()) {
			element->draw(getContextWindow()->getWindow());
		}
	}
}

void GameMenuScreen::draw() {
	for (auto element : getButtons()) {
		if (element->getIsVisible()) {
			element->draw(getContextWindow()->getWindow());
		}
	}

	for (auto element : getTexts()) {
		getContextWindow()->getWindow().draw(*element);
	}
}

void ActiveScreen::setContextWindow(ContextWindow* cw) {
	contextWindow_ = cw;
}

ContextWindow* ActiveScreen::getContextWindow() {
	return contextWindow_;
}

ActiveScreenType ActiveScreen::getScreenType() {
	return type_;
}

void ActiveScreen::setScreenType(ActiveScreenType type) {
	type_ = type;
}

MainMenuScreen::MainMenuScreen() {
	setScreenType(MAIN_MENU);
	mainMenuCreate();
}

void MainMenuScreen::mainMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	// if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.otf"))
		// std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonExit(new Button(EXIT, L"Wyjdź", {200, 100}, 30));
	buttonExit->setFont(getFont());
	buttonExit->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 300});
	buttonExit->setInactiveBackColor(sf::Color::Red);
	buttonExit->setInactiveTextColor(sf::Color::Black);
	buttonExit->setFocusBackColor(sf::Color::Black);
	buttonExit->setFocusTextColor(sf::Color::Red);

	std::shared_ptr<Button> buttonPlay(new Button(PLAY, L"Graj", {200, 100}, 30));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 100});
	buttonPlay->setInactiveBackColor(sf::Color::Red);
	buttonPlay->setInactiveTextColor(sf::Color::Black);
	buttonPlay->setFocusBackColor(sf::Color::Black);
	buttonPlay->setFocusTextColor(sf::Color::Red);

	addButton(buttonExit);
	addButton(buttonPlay);

	for (auto element : getButtons()) {
		element->setIsClicked(false);
		element->setIsVisible(true);
		element->setIsActive(false);
		element->setIsFocus(false);
	}
}

ScreenEventType MainMenuScreen::worker() {
	ScreenEventType eventType = IDLE;
	for (auto element : getButtons()) {
		if (element->isMouseOver(getContextWindow()->getWindow())) {
			element->mouseIsOver();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				element->setIsClicked(true);
			}
		} else {
			element->mouseIsNotOver();
		}
		if (element->getIsClicked()) {
			element->setIsClicked(false);
			return element->getEventType();
		}

		buttonSetColors(element);
	}
	return eventType;
}

ScreenEventType GameMenuScreen::worker() {
	ScreenEventType eventType = IDLE;
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
				setOtherButtonsInactive(element);
				buttonClickHandle(element);
				if (element->getEventType() == START_GAME) {
					element->setIsActive(false);  // in case of not starting game
				} else {
					element->setIsActive(true);
				}
				element->setIsClicked(false);
				return element->getEventType();
			}
		}

		buttonSetColors(element);
	}

	return eventType;
}

GameMenuScreen::GameMenuScreen() {
	setScreenType(GAME_MENU);
	gameMenuCreate();

	std::shared_ptr<playerSettings> player1Settings = std::make_shared<playerSettings>();
	player1Settings->isNone = true;
	player1Settings->isHuman = false;
	player1Settings->level = 1;
	playerSettingsList_.push_back(player1Settings);

	std::shared_ptr<playerSettings> player2Settings = std::make_shared<playerSettings>();
	player2Settings->isNone = true;
	player2Settings->isHuman = false;
	player2Settings->level = 1;
	playerSettingsList_.push_back(player2Settings);

	std::shared_ptr<playerSettings> player3Settings = std::make_shared<playerSettings>();
	player3Settings->isNone = true;
	player3Settings->isHuman = false;
	player3Settings->level = 1;
	playerSettingsList_.push_back(player3Settings);

	std::shared_ptr<playerSettings> player4Settings = std::make_shared<playerSettings>();
	player4Settings->isNone = true;
	player4Settings->isHuman = false;
	player4Settings->level = 1;
	playerSettingsList_.push_back(player4Settings);
}

void GameMenuScreen::gameMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	// if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.otf"))
		// std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonPlay(new Button(START_GAME, L"Graj", {200, 100}, 30));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f) - 200, 700});
	buttonPlay->setInactiveBackColor(sf::Color::Red);
	buttonPlay->setInactiveTextColor(sf::Color::Black);
	buttonPlay->setFocusBackColor(sf::Color::Black);
	buttonPlay->setFocusTextColor(sf::Color::Red);

	std::shared_ptr<Button> buttonReturn(new Button(RETURN_TO_MAIN_MENU, L"Powrót", {200, 100}, 30));
	buttonReturn->setFont(getFont());
	buttonReturn->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f) + 200, 700});
	buttonReturn->setInactiveBackColor(sf::Color::Red);
	buttonReturn->setInactiveTextColor(sf::Color::Black);
	buttonReturn->setFocusBackColor(sf::Color::Black);
	buttonReturn->setFocusTextColor(sf::Color::Red);

	addButton(buttonReturn);
	addButton(buttonPlay);

	for (auto element : getButtons()) {
		element->setIsClicked(false);
		element->setIsVisible(true);
		element->setIsActive(false);
		element->setIsFocus(false);
	}

	sf::Vector2f col1(getContextWindow()->getWindow().getSize().x / 2.0f - 300, 100);
	sf::Vector2f col2(getContextWindow()->getWindow().getSize().x / 2.0f - 100, 100);
	sf::Vector2f col3(getContextWindow()->getWindow().getSize().x / 2.0f + 100, 100);
	sf::Vector2f col4(getContextWindow()->getWindow().getSize().x / 2.0f + 300, 100);
	int y_step = 100;

	createPlayerSettingsColumn(1, col1, y_step);
	createPlayerSettingsColumn(2, col2, y_step);
	createPlayerSettingsColumn(3, col3, y_step);
	createPlayerSettingsColumn(4, col4, y_step);
}

void GameMenuScreen::createPlayerSettingsColumn(int colNum, sf::Vector2f posStart, int yStep) {
	int fontSize = 30;
	int fontSizeSmall = 25;
	std::string playerTextString = "Gracz X";
	sf::Color grey(192, 192, 192);
	sf::Vector2f buttonSize = sf::Vector2f(150, 100);
	sf::Vector2f buttonSizeSmall = sf::Vector2f(50, 100);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = grey;
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	sf::Color textColor = sf::Color::Black;
	sf::Color playerTextColor = sf::Color::Black;
	;

	ScreenEventType buttonNoneEvent = IDLE;
	ScreenEventType buttonHumanEvent = IDLE;
	ScreenEventType buttonAIEvent = IDLE;
	ScreenEventType buttonAILevel1Event = IDLE;
	ScreenEventType buttonAILevel2Event = IDLE;
	ScreenEventType buttonAILevel3Event = IDLE;
	switch (colNum) {
		case 1:
			playerTextColor = sf::Color::Green;
			playerTextString = "Gracz 1";
			buttonNoneEvent = PLAYER_1_SET_NONE;
			buttonHumanEvent = PLAYER_1_SET_HUMAN;
			buttonAIEvent = PLAYER_1_SET_AI;
			buttonAILevel1Event = PLAYER_1_SET_AI_LEVEL_1;
			buttonAILevel2Event = PLAYER_1_SET_AI_LEVEL_2;
			buttonAILevel3Event = PLAYER_1_SET_AI_LEVEL_3;
			break;

		case 2:
			playerTextColor = sf::Color::Red;
			playerTextString = "Gracz 2";
			buttonNoneEvent = PLAYER_2_SET_NONE;
			buttonHumanEvent = PLAYER_2_SET_HUMAN;
			buttonAIEvent = PLAYER_2_SET_AI;
			buttonAILevel1Event = PLAYER_2_SET_AI_LEVEL_1;
			buttonAILevel2Event = PLAYER_2_SET_AI_LEVEL_2;
			buttonAILevel3Event = PLAYER_2_SET_AI_LEVEL_3;
			break;

		case 3:
			playerTextColor = sf::Color::Blue;
			playerTextString = "Gracz 3";
			buttonNoneEvent = PLAYER_3_SET_NONE;
			buttonHumanEvent = PLAYER_3_SET_HUMAN;
			buttonAIEvent = PLAYER_3_SET_AI;
			buttonAILevel1Event = PLAYER_3_SET_AI_LEVEL_1;
			buttonAILevel2Event = PLAYER_3_SET_AI_LEVEL_2;
			buttonAILevel3Event = PLAYER_3_SET_AI_LEVEL_3;
			break;

		case 4:
			playerTextColor = sf::Color::Yellow;
			playerTextString = "Gracz 4";
			buttonNoneEvent = PLAYER_4_SET_NONE;
			buttonHumanEvent = PLAYER_4_SET_HUMAN;
			buttonAIEvent = PLAYER_4_SET_AI;
			buttonAILevel1Event = PLAYER_4_SET_AI_LEVEL_1;
			buttonAILevel2Event = PLAYER_4_SET_AI_LEVEL_2;
			buttonAILevel3Event = PLAYER_4_SET_AI_LEVEL_3;
			break;

		default:
			break;
	}
	std::shared_ptr<sf::Text> PlayerText(new sf::Text(playerTextString, getFont(), fontSize));
	PlayerText->setOrigin(PlayerText->getGlobalBounds().getSize() / 2.f + PlayerText->getLocalBounds().getPosition());
	PlayerText->setPosition(posStart);
	PlayerText->setFillColor(playerTextColor);
	PlayerText->setOutlineColor(textColor);
	PlayerText->setOutlineThickness(3);
	addText(PlayerText);

	std::shared_ptr<Button> buttonPlayerSetNone(new Button(buttonNoneEvent, "Brak", buttonSize, fontSize));
	buttonPlayerSetNone->setFont(getFont());
	buttonPlayerSetNone->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep));
	buttonPlayerSetNone->setActiveBackColor(activeButtonBackColor);
	buttonPlayerSetNone->setActiveTextColor(activeButtonTextColor);
	buttonPlayerSetNone->setInactiveBackColor(inActiveButtonBackColor);
	buttonPlayerSetNone->setInactiveTextColor(inActiveButtonTextColor);
	buttonPlayerSetNone->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetNone->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetNone->setIsClicked(false);
	buttonPlayerSetNone->setIsVisible(true);
	buttonPlayerSetNone->setIsActive(true);
	buttonPlayerSetNone->setIsFocus(false);
	addButton(buttonPlayerSetNone);

	std::shared_ptr<Button> buttonPlayerSetHuman(new Button(buttonHumanEvent, L"Człowiek", buttonSize, fontSize));
	buttonPlayerSetHuman->setFont(getFont());
	buttonPlayerSetHuman->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 2));
	buttonPlayerSetHuman->setActiveBackColor(activeButtonBackColor);
	buttonPlayerSetHuman->setActiveTextColor(activeButtonTextColor);
	buttonPlayerSetHuman->setInactiveBackColor(inActiveButtonBackColor);
	buttonPlayerSetHuman->setInactiveTextColor(inActiveButtonTextColor);
	buttonPlayerSetHuman->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetHuman->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetHuman->setIsClicked(false);
	buttonPlayerSetHuman->setIsVisible(true);
	buttonPlayerSetHuman->setIsActive(false);
	buttonPlayerSetHuman->setIsFocus(false);
	addButton(buttonPlayerSetHuman);

	std::shared_ptr<Button> buttonPlayerSetAI(new Button(buttonAIEvent, "AI", buttonSize, fontSize));
	buttonPlayerSetAI->setFont(getFont());
	buttonPlayerSetAI->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 3));
	buttonPlayerSetAI->setActiveBackColor(activeButtonBackColor);
	buttonPlayerSetAI->setActiveTextColor(activeButtonTextColor);
	buttonPlayerSetAI->setInactiveBackColor(inActiveButtonBackColor);
	buttonPlayerSetAI->setInactiveTextColor(inActiveButtonTextColor);
	buttonPlayerSetAI->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAI->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAI->setIsClicked(false);
	buttonPlayerSetAI->setIsVisible(true);
	buttonPlayerSetAI->setIsActive(false);
	buttonPlayerSetAI->setIsFocus(false);
	addButton(buttonPlayerSetAI);

	std::shared_ptr<sf::Text> PlayerTextAILevel(new sf::Text("Poziom AI", getFont(), fontSizeSmall));
	PlayerTextAILevel->setOrigin(PlayerTextAILevel->getGlobalBounds().getSize() / 2.f +
								 PlayerTextAILevel->getLocalBounds().getPosition());  // text origin at center
	PlayerTextAILevel->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 3.8));
	PlayerTextAILevel->setFillColor(textColor);
	addText(PlayerTextAILevel);

	std::shared_ptr<Button> buttonPlayerSetAILevel1(new Button(buttonAILevel1Event, "1", buttonSizeSmall, fontSize));
	buttonPlayerSetAILevel1->setFont(getFont());
	buttonPlayerSetAILevel1->setPosition(sf::Vector2f(posStart.x - buttonSizeSmall.x, posStart.y + yStep * 4.5));
	buttonPlayerSetAILevel1->setActiveBackColor(activeButtonBackColor);
	buttonPlayerSetAILevel1->setActiveTextColor(activeButtonTextColor);
	buttonPlayerSetAILevel1->setInactiveBackColor(inActiveButtonBackColor);
	buttonPlayerSetAILevel1->setInactiveTextColor(inActiveButtonTextColor);
	buttonPlayerSetAILevel1->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAILevel1->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAILevel1->setIsClicked(false);
	buttonPlayerSetAILevel1->setIsVisible(false);
	buttonPlayerSetAILevel1->setIsActive(true);
	buttonPlayerSetAILevel1->setIsFocus(false);
	addButton(buttonPlayerSetAILevel1);

	std::shared_ptr<Button> buttonPlayerSetAILevel2(new Button(buttonAILevel2Event, "2", buttonSizeSmall, fontSize));
	buttonPlayerSetAILevel2->setFont(getFont());
	buttonPlayerSetAILevel2->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 4.5));
	buttonPlayerSetAILevel2->setActiveBackColor(activeButtonBackColor);
	buttonPlayerSetAILevel2->setActiveTextColor(activeButtonTextColor);
	buttonPlayerSetAILevel2->setInactiveBackColor(inActiveButtonBackColor);
	buttonPlayerSetAILevel2->setInactiveTextColor(inActiveButtonTextColor);
	buttonPlayerSetAILevel2->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAILevel2->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAILevel2->setIsClicked(false);
	buttonPlayerSetAILevel2->setIsVisible(false);
	buttonPlayerSetAILevel2->setIsActive(false);
	buttonPlayerSetAILevel2->setIsFocus(false);
	addButton(buttonPlayerSetAILevel2);

	std::shared_ptr<Button> buttonPlayerSetAILevel3(new Button(buttonAILevel3Event, "3", buttonSizeSmall, fontSize));
	buttonPlayerSetAILevel3->setFont(getFont());
	buttonPlayerSetAILevel3->setPosition(sf::Vector2f(posStart.x + buttonSizeSmall.x, posStart.y + yStep * 4.5));
	buttonPlayerSetAILevel3->setActiveBackColor(activeButtonBackColor);
	buttonPlayerSetAILevel3->setActiveTextColor(activeButtonTextColor);
	buttonPlayerSetAILevel3->setInactiveBackColor(inActiveButtonBackColor);
	buttonPlayerSetAILevel3->setInactiveTextColor(inActiveButtonTextColor);
	buttonPlayerSetAILevel3->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAILevel3->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAILevel3->setIsClicked(false);
	buttonPlayerSetAILevel3->setIsVisible(false);
	buttonPlayerSetAILevel3->setIsActive(false);
	buttonPlayerSetAILevel3->setIsFocus(false);
	addButton(buttonPlayerSetAILevel3);
}

void GameMenuScreen::setPlayerSettings(unsigned int index, bool isNone, bool isHuman, int level) {
	if (index < playerSettingsList_.size())	 // check index param
	{
		playerSettingsList_[index]->isNone = isNone;
		playerSettingsList_[index]->isHuman = isHuman;
		playerSettingsList_[index]->level = level;
	}
}

std::vector<std::shared_ptr<Button>>& ActiveScreen::getButtons() {
	return buttons_;
}

std::vector<std::shared_ptr<sf::Text>>& ActiveScreen::getTexts() {
	return texts_;
}

void GameMenuScreen::buttonClickHandle(std::shared_ptr<Button> buttonPtr) {
	switch (buttonPtr->getEventType()) {
		case PLAYER_1_SET_NONE:
			setPlayerSettings(0, true, false, 1);
			setAILevelColumnVisibility(1, false);
			break;
		case PLAYER_2_SET_NONE:
			setPlayerSettings(1, true, false, 1);
			setAILevelColumnVisibility(2, false);
			break;
		case PLAYER_3_SET_NONE:
			setPlayerSettings(2, true, false, 1);
			setAILevelColumnVisibility(3, false);
			break;
		case PLAYER_4_SET_NONE:
			setPlayerSettings(3, true, false, 1);
			setAILevelColumnVisibility(4, false);
			break;

		case PLAYER_1_SET_HUMAN:
			setPlayerSettings(0, false, true, 1);
			setAILevelColumnVisibility(1, false);
			break;
		case PLAYER_2_SET_HUMAN:
			setPlayerSettings(1, false, true, 1);
			setAILevelColumnVisibility(2, false);
			break;
		case PLAYER_3_SET_HUMAN:
			setPlayerSettings(2, false, true, 1);
			setAILevelColumnVisibility(3, false);
			break;
		case PLAYER_4_SET_HUMAN:
			setPlayerSettings(3, false, true, 1);
			setAILevelColumnVisibility(4, false);
			break;

		case PLAYER_1_SET_AI:
			setPlayerSettings(0, false, false, 1);
			setAILevelColumnVisibility(1, true);
			setDefaultAILevelButtonsFocus(1);
			break;
		case PLAYER_2_SET_AI:
			setPlayerSettings(1, false, false, 1);
			setAILevelColumnVisibility(2, true);
			setDefaultAILevelButtonsFocus(2);
			break;
		case PLAYER_3_SET_AI:
			setPlayerSettings(2, false, false, 1);
			setAILevelColumnVisibility(3, true);
			setDefaultAILevelButtonsFocus(3);
			break;
		case PLAYER_4_SET_AI:
			setPlayerSettings(3, false, false, 1);
			setAILevelColumnVisibility(4, true);
			setDefaultAILevelButtonsFocus(4);
			break;

		case PLAYER_1_SET_AI_LEVEL_1:
			setPlayerSettings(0, false, false, 1);
			break;
		case PLAYER_2_SET_AI_LEVEL_1:
			setPlayerSettings(1, false, false, 1);
			break;
		case PLAYER_3_SET_AI_LEVEL_1:
			setPlayerSettings(2, false, false, 1);
			break;
		case PLAYER_4_SET_AI_LEVEL_1:
			setPlayerSettings(3, false, false, 1);
			break;

		case PLAYER_1_SET_AI_LEVEL_2:
			setPlayerSettings(0, false, false, 2);
			break;
		case PLAYER_2_SET_AI_LEVEL_2:
			setPlayerSettings(1, false, false, 2);
			break;
		case PLAYER_3_SET_AI_LEVEL_2:
			setPlayerSettings(2, false, false, 2);
			break;
		case PLAYER_4_SET_AI_LEVEL_2:
			setPlayerSettings(3, false, false, 2);
			break;

		case PLAYER_1_SET_AI_LEVEL_3:
			setPlayerSettings(0, false, false, 3);
			break;
		case PLAYER_2_SET_AI_LEVEL_3:
			setPlayerSettings(1, false, false, 3);
			break;
		case PLAYER_3_SET_AI_LEVEL_3:
			setPlayerSettings(2, false, false, 3);
			break;
		case PLAYER_4_SET_AI_LEVEL_3:
			setPlayerSettings(3, false, false, 3);
			break;
		default:
			break;
	}
}

void GameMenuScreen::setAILevelColumnVisibility(int playerNum, bool visible) {
	for (auto element : getButtons()) {
		if (playerNum == getPlayerNumFromEventType(element->getEventType())) {
			if (isEventTypeAILevel(playerNum, element->getEventType())) {
				element->setIsVisible(visible);
			}
		}
	}
}

void GameMenuScreen::setOtherButtonsInactive(std::shared_ptr<Button> buttonPtr) {
	int playerNum = getPlayerNumFromEventType(buttonPtr->getEventType());
	for (auto element : getButtons()) {
		if (playerNum == getPlayerNumFromEventType(element->getEventType())) {
			if (!(isEventTypeAILevel(playerNum, buttonPtr->getEventType()) &&
					isEventTypeSetAI(playerNum, element->getEventType()))) {
				element->setIsActive(false);
			}
		}
	}
}

bool GameMenuScreen::isEventTypeSetAI(int playerNum, ScreenEventType event) {
	switch (playerNum) {
		case 1:
			if (event == PLAYER_1_SET_AI) {
				return true;
			}
			break;
		case 2:
			if (event == PLAYER_2_SET_AI) {
				return true;
			}
			break;
		case 3:
			if (event == PLAYER_3_SET_AI) {
				return true;
			}
			break;
		case 4:
			if (event == PLAYER_4_SET_AI) {
				return true;
			}
			break;
		default:
			return false;
			break;
	}
	return false;
}

bool GameMenuScreen::isEventTypeAILevel(int playerNum, ScreenEventType event) {
	switch (playerNum) {
		case 1:
			if ((event == PLAYER_1_SET_AI_LEVEL_1 || event == PLAYER_1_SET_AI_LEVEL_2 ||
					event == PLAYER_1_SET_AI_LEVEL_3)) {
				return true;
			}
			break;
		case 2:
			if ((event == PLAYER_2_SET_AI_LEVEL_1 || event == PLAYER_2_SET_AI_LEVEL_2 ||
					event == PLAYER_2_SET_AI_LEVEL_3)) {
				return true;
			}
			break;
		case 3:
			if ((event == PLAYER_3_SET_AI_LEVEL_1 || event == PLAYER_3_SET_AI_LEVEL_2 ||
					event == PLAYER_3_SET_AI_LEVEL_3)) {
				return true;
			}
			break;
		case 4:
			if ((event == PLAYER_4_SET_AI_LEVEL_1 || event == PLAYER_4_SET_AI_LEVEL_2 ||
					event == PLAYER_4_SET_AI_LEVEL_3)) {
				return true;
			}
			break;
		default:
			return false;
			break;
	}
	return false;
}

int GameMenuScreen::getPlayerNumFromEventType(ScreenEventType event) {
	if (event == PLAYER_1_SET_AI || event == PLAYER_1_SET_AI_LEVEL_1 || event == PLAYER_1_SET_AI_LEVEL_2 ||
		event == PLAYER_1_SET_AI_LEVEL_3 || event == PLAYER_1_SET_HUMAN || event == PLAYER_1_SET_NONE) {
		return 1;
	} else if (event == PLAYER_2_SET_AI || event == PLAYER_2_SET_AI_LEVEL_1 || event == PLAYER_2_SET_AI_LEVEL_2 ||
			   event == PLAYER_2_SET_AI_LEVEL_3 || event == PLAYER_2_SET_HUMAN || event == PLAYER_2_SET_NONE) {
		return 2;
	} else if (event == PLAYER_3_SET_AI || event == PLAYER_3_SET_AI_LEVEL_1 || event == PLAYER_3_SET_AI_LEVEL_2 ||
			   event == PLAYER_3_SET_AI_LEVEL_3 || event == PLAYER_3_SET_HUMAN || event == PLAYER_3_SET_NONE) {
		return 3;
	} else if (event == PLAYER_4_SET_AI || event == PLAYER_4_SET_AI_LEVEL_1 || event == PLAYER_4_SET_AI_LEVEL_2 ||
			   event == PLAYER_4_SET_AI_LEVEL_3 || event == PLAYER_4_SET_HUMAN || event == PLAYER_4_SET_NONE) {
		return 4;
	}

	return 0;
}

void GameMenuScreen::setDefaultAILevelButtonsFocus(int playerNum) {
	ScreenEventType level1Event;
	ScreenEventType level2Event;
	ScreenEventType level3Event;
	switch (playerNum) {
		case 1:
			level1Event = PLAYER_1_SET_AI_LEVEL_1;
			level2Event = PLAYER_1_SET_AI_LEVEL_2;
			level3Event = PLAYER_1_SET_AI_LEVEL_3;
			break;
		case 2:
			level1Event = PLAYER_2_SET_AI_LEVEL_1;
			level2Event = PLAYER_2_SET_AI_LEVEL_2;
			level3Event = PLAYER_2_SET_AI_LEVEL_3;
			break;
		case 3:
			level1Event = PLAYER_3_SET_AI_LEVEL_1;
			level2Event = PLAYER_3_SET_AI_LEVEL_2;
			level3Event = PLAYER_3_SET_AI_LEVEL_3;
			break;
		case 4:
			level1Event = PLAYER_4_SET_AI_LEVEL_1;
			level2Event = PLAYER_4_SET_AI_LEVEL_2;
			level3Event = PLAYER_4_SET_AI_LEVEL_3;
			break;
		default:
			return;
	}
	for (auto element : getButtons()) {
		if (element->getEventType() == level1Event) {
			element->setIsActive(true);
		} else if (element->getEventType() == level2Event || element->getEventType() == level3Event) {
			element->setIsActive(false);
		}
	}
}

void ActiveScreen::buttonSetColors(std::shared_ptr<Button> buttonPtr) {
	buttonPtr->updateColors();
}

std::vector<std::shared_ptr<playerSettings>> GameMenuScreen::getPlayersSettings() const {
	return playerSettingsList_;
}

std::vector<std::shared_ptr<playerSettings>> ActiveScreen::getPlayersSettings() const {
	std::vector<std::shared_ptr<playerSettings>> empty;
	return empty;
}

std::vector<std::shared_ptr<Player>> ActiveScreen::getPlayersResult() {
	std::vector<std::shared_ptr<Player>> empty;
	return empty;
}