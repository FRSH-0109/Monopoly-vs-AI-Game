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
	std::cout << "MMS deconstructor" << std::endl;
}

void MainMenuScreen::mainMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonExit(new Button(Exit, "Exit", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonExit->setFont(getFont());
	buttonExit->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 300});

	std::shared_ptr<Button> buttonPlay(new Button(Play, "Play", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 100});

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
}

GameMenuScreen::~GameMenuScreen() {
	std::cout << "GMS deconstructor" << std::endl;
}

void GameMenuScreen::gameMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonPlay(
		new Button(StartGame, "Start game", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f), 100});

	std::shared_ptr<Button> buttonAddPlayer(
		new Button(AddPlayer, "Add Player", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonAddPlayer->setFont(getFont());
	buttonAddPlayer->setPosition(
		{(getContextWindow()->getWindow().getSize().x / 2.0f) - buttonAddPlayer->getSize().x, 300});

	std::shared_ptr<Button> buttonRemovePlayer(
		new Button(RemovePlayer, "Remove Player", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonRemovePlayer->setFont(getFont());
	buttonRemovePlayer->setPosition(
		{(getContextWindow()->getWindow().getSize().x / 2.0f) + buttonRemovePlayer->getSize().x, 300});

	std::shared_ptr<Button> buttonAddAIPlayer(
		new Button(AddAIPlayer, "Add AI player", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonAddAIPlayer->setFont(getFont());
	buttonAddAIPlayer->setPosition(
		{(getContextWindow()->getWindow().getSize().x / 2.0f) - buttonAddAIPlayer->getSize().x, 500});

	std::shared_ptr<Button> buttonRemoveAIPlayer(
		new Button(RemoveAIPlayer, "Remove AI Player", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonRemoveAIPlayer->setFont(getFont());
	buttonRemoveAIPlayer->setPosition(
		{(getContextWindow()->getWindow().getSize().x / 2.0f) + buttonRemoveAIPlayer->getSize().x, 500});

	std::shared_ptr<Button> buttonReturn(
		new Button(ReturnToMainMenu, "Return", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonReturn->setFont(getFont());
	buttonReturn->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f), 700});

	addButton(buttonAddPlayer);
	addButton(buttonRemovePlayer);
	addButton(buttonAddAIPlayer);
	addButton(buttonRemoveAIPlayer);
	addButton(buttonReturn);
	addButton(buttonPlay);

	for (auto element : getButtons()) {
		element->setIsClicked(false);
	}

	// std::shared_ptr<sf::Text> text1("PLAYERNUM", getFont(), 30);
	// text1->setPosition((getContextWindow()->getWindow().getSize().x / 2.0f), 300);
	// text1->setColor(sf::Color::Black);
	// addText(text1);
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