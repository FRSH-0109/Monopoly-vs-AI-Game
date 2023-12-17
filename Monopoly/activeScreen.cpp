#include "activeScreen.h"

ActiveScreen::ActiveScreen() {}
ActiveScreen::~ActiveScreen() {}

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

void ActiveScreen::draw() {
	for (auto element : buttons_) {
		element->draw(getContextWindow()->getWindow());
	}

	for (auto element : texts_) {
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
	std::cout << "MMS constructor" << std::endl;
	setScreenType(MainMenu);
	mainMenuCreate();
}

MainMenuScreen::~MainMenuScreen() {
	std::cout << "MMS destructor" << std::endl;
}

void MainMenuScreen::mainMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonExit(new Button(Exit, "Exit", {200, 100}, 30));
	buttonExit->setFont(getFont());
	buttonExit->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 300});
	buttonExit->setMainBackColor(sf::Color::Red);
	buttonExit->setMainTextColor(sf::Color::Black);
	buttonExit->setFocusBackColor(sf::Color::Black);
	buttonExit->setFocusTextColor(sf::Color::Red);

	std::shared_ptr<Button> buttonPlay(new Button(Play, "Play", {200, 100}, 30));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 100});
	buttonPlay->setMainBackColor(sf::Color::Red);
	buttonPlay->setMainTextColor(sf::Color::Black);
	buttonPlay->setFocusBackColor(sf::Color::Black);
	buttonPlay->setFocusTextColor(sf::Color::Red);

	addButton(buttonExit);
	addButton(buttonPlay);

	for (auto element : getButtons()) {
		element->setIsClicked(false);
	}
}

ScreenEventType MainMenuScreen::worker() {
	ScreenEventType eventType = Idle;
	for (auto element : getButtons()) {
		if (element->getIsClicked()) {
			element->setIsClicked(false);
			return element->getEventType();
		}
	}
	return eventType;
}

ScreenEventType GameMenuScreen::worker() {
	ScreenEventType eventType = Idle;
	for (auto element : getButtons()) {
		if (element->getIsClicked()) {
			element->setIsClicked(false);
			return element->getEventType();
		}
	}
	return eventType;
}

GameMenuScreen::GameMenuScreen() {
	std::cout << "GMS constructor" << std::endl;
	setScreenType(GameMenu);
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
	player3Settings->isNone = false;
	player3Settings->isHuman = false;
	player3Settings->level = 1;
	playerSettingsList_.push_back(player3Settings);

	std::shared_ptr<playerSettings> player4Settings = std::make_shared<playerSettings>();
	player4Settings->isNone = false;
	player4Settings->isHuman = false;
	player4Settings->level = 1;
	playerSettingsList_.push_back(player4Settings);
}

GameMenuScreen::~GameMenuScreen() {
	std::cout << "GMS destructor" << std::endl;
}

void GameMenuScreen::gameMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonPlay(new Button(StartGame, "Start game", {200, 100}, 30));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f), 100});
	buttonPlay->setMainBackColor(sf::Color::Red);
	buttonPlay->setMainTextColor(sf::Color::Black);
	buttonPlay->setFocusBackColor(sf::Color::Black);
	buttonPlay->setFocusTextColor(sf::Color::Red);

	std::shared_ptr<Button> buttonReturn(new Button(ReturnToMainMenu, "Return", {200, 100}, 30));
	buttonReturn->setFont(getFont());
	buttonReturn->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f), 900});
	buttonReturn->setMainBackColor(sf::Color::Red);
	buttonReturn->setMainTextColor(sf::Color::Black);
	buttonReturn->setFocusBackColor(sf::Color::Black);
	buttonReturn->setFocusTextColor(sf::Color::Red);

	addButton(buttonReturn);
	addButton(buttonPlay);

	for (auto element : getButtons()) {
		element->setIsClicked(false);
		element->setIsVisible(true);
	}

	sf::Vector2f col1(getContextWindow()->getWindow().getSize().x / 2.0f - 300, 200);
	sf::Vector2f col2(getContextWindow()->getWindow().getSize().x / 2.0f - 100, 200);
	sf::Vector2f col3(getContextWindow()->getWindow().getSize().x / 2.0f + 100, 200);
	sf::Vector2f col4(getContextWindow()->getWindow().getSize().x / 2.0f + 300, 200);
	int y_step = 100;

	createPlayerSettingsColumn(1, col1, y_step);
	createPlayerSettingsColumn(2, col2, y_step);
	createPlayerSettingsColumn(3, col3, y_step);
	createPlayerSettingsColumn(4, col4, y_step);
}

void GameMenuScreen::createPlayerSettingsColumn(int colNum, sf::Vector2f posStart, int yStep) {
	int fontSize = 30;
	std::string playerTextString = "Player X";
	sf::Vector2f buttonSize = sf::Vector2f(150, 100);
	sf::Vector2f buttonSizeSmall = sf::Vector2f(50, 100);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color::White;
	sf::Color FocusButtonBackColor = sf::Color::Black;

	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	sf::Color textColor = sf::Color::Black;

	ScreenEventType buttonNoneEvent = Idle;
	ScreenEventType buttonHumanEvent = Idle;
	ScreenEventType buttonAIEvent = Idle;
	ScreenEventType buttonAILevel1Event = Idle;
	ScreenEventType buttonAILevel2Event = Idle;
	ScreenEventType buttonAILevel3Event = Idle;
	switch (colNum) {
		case 1:
			playerTextString = "Player 1";
			buttonNoneEvent = Player1SetNone;
			buttonHumanEvent = Player1SetHuman;
			buttonAIEvent = Player1SetAI;
			buttonAILevel1Event = Player1SetAILevel1;
			buttonAILevel2Event = Player1SetAILevel2;
			buttonAILevel3Event = Player1SetAILevel3;
			break;

		case 2:
			playerTextString = "Player 2";
			buttonNoneEvent = Player2SetNone;
			buttonHumanEvent = Player2SetHuman;
			buttonAIEvent = Player2SetAI;
			buttonAILevel1Event = Player2SetAILevel1;
			buttonAILevel2Event = Player2SetAILevel2;
			buttonAILevel3Event = Player2SetAILevel3;
			break;

		case 3:
			playerTextString = "Player 3";
			buttonNoneEvent = Player3SetNone;
			buttonHumanEvent = Player3SetHuman;
			buttonAIEvent = Player3SetAI;
			buttonAILevel1Event = Player3SetAILevel1;
			buttonAILevel2Event = Player3SetAILevel2;
			buttonAILevel3Event = Player3SetAILevel3;
			break;

		case 4:
			playerTextString = "Player 4";
			buttonNoneEvent = Player4SetNone;
			buttonHumanEvent = Player4SetHuman;
			buttonAIEvent = Player4SetAI;
			buttonAILevel1Event = Player4SetAILevel1;
			buttonAILevel2Event = Player4SetAILevel2;
			buttonAILevel3Event = Player4SetAILevel3;
			break;

		default:
			break;
	}
	std::shared_ptr<sf::Text> PlayerText(new sf::Text(playerTextString, getFont(), fontSize));
	PlayerText->setOrigin(PlayerText->getGlobalBounds().getSize() / 2.f + PlayerText->getLocalBounds().getPosition());
	PlayerText->setPosition(posStart);
	PlayerText->setColor(textColor);
	addText(PlayerText);

	std::shared_ptr<Button> buttonPlayerSetNone(new Button(buttonNoneEvent, "None", buttonSize, fontSize));
	buttonPlayerSetNone->setFont(getFont());
	buttonPlayerSetNone->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep));
	buttonPlayerSetNone->setMainBackColor(activeButtonBackColor);
	buttonPlayerSetNone->setMainTextColor(activeButtonTextColor);
	buttonPlayerSetNone->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetNone->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetNone->setIsClicked(false);
	buttonPlayerSetNone->setIsVisible(true);
	addButton(buttonPlayerSetNone);

	std::shared_ptr<Button> buttonPlayerSetHuman(new Button(buttonNoneEvent, "Human", buttonSize, fontSize));
	buttonPlayerSetHuman->setFont(getFont());
	buttonPlayerSetHuman->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 2));
	buttonPlayerSetHuman->setMainBackColor(inActiveButtonBackColor);
	buttonPlayerSetHuman->setMainTextColor(inActiveButtonTextColor);
	buttonPlayerSetHuman->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetHuman->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetHuman->setIsClicked(false);
	buttonPlayerSetHuman->setIsVisible(true);
	addButton(buttonPlayerSetHuman);

	std::shared_ptr<Button> buttonPlayerSetAI(new Button(buttonAIEvent, "AI", buttonSize, fontSize));
	buttonPlayerSetAI->setFont(getFont());
	buttonPlayerSetAI->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 3));
	buttonPlayerSetAI->setMainBackColor(inActiveButtonBackColor);
	buttonPlayerSetAI->setMainTextColor(inActiveButtonTextColor);
	buttonPlayerSetAI->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAI->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAI->setIsClicked(false);
	buttonPlayerSetAI->setIsVisible(true);
	addButton(buttonPlayerSetAI);

	std::shared_ptr<sf::Text> PlayerTextAILevel(new sf::Text("AI level", getFont(), fontSize));
	PlayerTextAILevel->setOrigin(PlayerTextAILevel->getGlobalBounds().getSize() / 2.f +
								 PlayerTextAILevel->getLocalBounds().getPosition());  // text origin at center
	PlayerTextAILevel->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 4));
	PlayerTextAILevel->setColor(textColor);
	addText(PlayerTextAILevel);

	std::shared_ptr<Button> buttonPlayerSetAILevel1(new Button(buttonAILevel1Event, "1", buttonSizeSmall, fontSize));
	buttonPlayerSetAILevel1->setFont(getFont());
	buttonPlayerSetAILevel1->setPosition(sf::Vector2f(posStart.x - buttonSizeSmall.x, posStart.y + yStep * 5));
	buttonPlayerSetAILevel1->setMainBackColor(activeButtonBackColor);
	buttonPlayerSetAILevel1->setMainTextColor(activeButtonTextColor);
	buttonPlayerSetAILevel1->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAILevel1->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAILevel1->setIsClicked(false);
	buttonPlayerSetAILevel1->setIsVisible(false);
	addButton(buttonPlayerSetAILevel1);

	std::shared_ptr<Button> buttonPlayerSetAILevel2(new Button(buttonAILevel2Event, "2", buttonSizeSmall, fontSize));
	buttonPlayerSetAILevel2->setFont(getFont());
	buttonPlayerSetAILevel2->setPosition(sf::Vector2f(posStart.x, posStart.y + yStep * 5));
	buttonPlayerSetAILevel2->setMainBackColor(inActiveButtonBackColor);
	buttonPlayerSetAILevel2->setMainTextColor(inActiveButtonTextColor);
	buttonPlayerSetAILevel2->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAILevel2->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAILevel2->setIsClicked(false);
	buttonPlayerSetAILevel2->setIsVisible(false);
	addButton(buttonPlayerSetAILevel2);

	std::shared_ptr<Button> buttonPlayerSetAILevel3(new Button(buttonAILevel3Event, "3", buttonSizeSmall, fontSize));
	buttonPlayerSetAILevel3->setFont(getFont());
	buttonPlayerSetAILevel3->setPosition(sf::Vector2f(posStart.x + buttonSizeSmall.x, posStart.y + yStep * 5));
	buttonPlayerSetAILevel3->setMainBackColor(inActiveButtonBackColor);
	buttonPlayerSetAILevel3->setMainTextColor(inActiveButtonTextColor);
	buttonPlayerSetAILevel3->setFocusBackColor(FocusButtonBackColor);
	buttonPlayerSetAILevel3->setFocusTextColor(FocusButtonTextColor);
	buttonPlayerSetAILevel3->setIsClicked(false);
	buttonPlayerSetAILevel3->setIsVisible(false);
	addButton(buttonPlayerSetAILevel3);
}

void GameMenuScreen::setPlayerSettings(int index, bool isNone, bool isHuman, int level) {
	if (index >= 0 && index < playerSettingsList_.size())  // check index param
	{
		playerSettingsList_[index]->isNone = isNone;
		playerSettingsList_[index]->isHuman = isHuman;
		playerSettingsList_[index]->level = level;
	}
}

std::vector<std::shared_ptr<Button>>& ActiveScreen::getButtons() {
	return buttons_;
}

void GameMenuScreen::pollForEvents(sf::Event& event) {
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

void MainMenuScreen::pollForEvents(sf::Event& event) {
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

void MainMenuScreen::eventHandle(ScreenEventType eventType) {}

void GameMenuScreen::eventHandle(ScreenEventType eventType) {
	switch (eventType) {
		case Player1SetNone:
			setPlayerSettings(0, true, false, 1);
			break;
		case Player2SetNone:
			setPlayerSettings(1, true, false, 1);
			break;
		case Player3SetNone:
			setPlayerSettings(2, true, false, 1);
			break;
		case Player4SetNone:
			setPlayerSettings(3, true, false, 1);
			break;

		case Player1SetHuman:
			setPlayerSettings(0, false, true, 1);
			break;
		case Player2SetHuman:
			setPlayerSettings(1, false, true, 1);
			break;
		case Player3SetHuman:
			setPlayerSettings(2, false, true, 1);
			break;
		case Player4SetHuman:
			setPlayerSettings(3, false, true, 1);
			break;

		case Player1SetAI:
			setPlayerSettings(0, false, false, 1);
			break;
		case Player2SetAI:
			setPlayerSettings(1, false, false, 1);
			break;
		case Player3SetAI:
			setPlayerSettings(2, false, false, 1);
			break;
		case Player4SetAI:
			setPlayerSettings(3, false, false, 1);
			break;

		case Player1SetAILevel1:
			setPlayerSettings(0, false, true, 1);
			break;
		case Player2SetAILevel1:
			setPlayerSettings(1, false, true, 1);
			break;
		case Player3SetAILevel1:
			setPlayerSettings(2, false, true, 1);
			break;
		case Player4SetAILevel1:
			setPlayerSettings(3, false, true, 1);
			break;

		case Player1SetAILevel2:
			setPlayerSettings(0, false, true, 2);
			break;
		case Player2SetAILevel2:
			setPlayerSettings(1, false, true, 2);
			break;
		case Player3SetAILevel2:
			setPlayerSettings(2, false, true, 2);
			break;
		case Player4SetAILevel2:
			setPlayerSettings(3, false, true, 2);
			break;

		case Player1SetAILevel3:
			setPlayerSettings(0, false, true, 3);
			break;
		case Player2SetAILevel3:
			setPlayerSettings(1, false, true, 3);
			break;
		case Player3SetAILevel3:
			setPlayerSettings(2, false, true, 3);
			break;
		case Player4SetAILevel3:
			setPlayerSettings(3, false, true, 3);
			break;
	}
}