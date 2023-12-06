#include "menuScreen.h"

void MenuScreen::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& MenuScreen::getFont() {
	return font_;
}

void MenuScreen::addButton(std::shared_ptr<Button> buttonTmp) {
	std::shared_ptr<Button> button = buttonTmp;
	buttons_.push_back(button);
}

void MenuScreen::draw() {
	for (auto element : buttons_) {
		element->draw(getContextWindow()->getWindow());
	}
}

void MenuScreen::setContextWindow(ContextWindow* cw) {
	contextWindow_ = cw;
}

ContextWindow* MenuScreen::getContextWindow() {
	return contextWindow_;
}

ActiveScreenType MenuScreen::getScreenType()
{
	return type_;
}

void MenuScreen::setScreenType(ActiveScreenType type)
{
	type_ = type;
}

MainMenuScreen::MainMenuScreen() {
	setScreenType(MainMenu);
	mainMenuCreate();
}

void MainMenuScreen::mainMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonExit(new Button(
		ExitButton, "Exit", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonExit->setFont(getFont());
	buttonExit->setPosition(
		{getContextWindow()->getWindow().getSize().x / 2.0f, 300});

	std::shared_ptr<Button> buttonPlay(new Button(
		PlayButton, "Play", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonPlay->setFont(getFont()); 
	buttonPlay->setPosition(
		{getContextWindow()->getWindow().getSize().x / 2.0f, 100});

	addButton(buttonExit);
	addButton(buttonPlay);
}

ScreenEventType MainMenuScreen::worker() {
	ScreenEventType data = Idle;
	return data;
}

ScreenEventType GameMenuScreen::worker() {
	ScreenEventType data = Idle;
	return data;
}

GameMenuScreen::GameMenuScreen() {
	setScreenType(GameMenu);
	gameMenuCreate();
}

void GameMenuScreen::gameMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonExit(new Button(ExitButton, "Players",
		{100, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonExit->setFont(getFont());
	buttonExit->setPosition(
		{getContextWindow()->getWindow().getSize().x / 2.0f, 300});

	std::shared_ptr<Button> buttonPlay(new Button(PlayButton, "Ai players",
		{100, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition(
		{getContextWindow()->getWindow().getSize().x / 2.0f, 100});

	addButton(buttonExit);
	addButton(buttonPlay);
}

std::vector<std::shared_ptr<Button>>& MenuScreen::getButtons() {
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