/**
 * @file main.cpp
 *
 * @brief Source file for main menu class
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "mainMenu.h"

void Menu::create() {
	contextWindow_ = ContextWindow::GetInstance();

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonExit(
		new ButtonExit("Exit", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonExit->setFont(getFont());
	buttonExit->setPosition(
		{getContextWindow()->getWindow().getSize().x / 2.0f, 300});

	std::shared_ptr<Button> buttonPlay(
		new ButtonPlay("Play", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition(
		{getContextWindow()->getWindow().getSize().x / 2.0f, 100});

	addButton(buttonExit);
	addButton(buttonPlay);
}

ContextWindow* Menu::getContextWindow() {
	return contextWindow_;
}

void Menu::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& Menu::getFont() {
	return font_;
}

void Menu::addButton(std::shared_ptr<Button> buttonTmp) {
	std::shared_ptr<Button> button = buttonTmp;
	buttons_.push_back(button);
}

void Menu::draw() {
	for (auto element : buttons_) {
		element->draw(getContextWindow()->getWindow());
	}
}

void Menu::pollForEvents(sf::Event& event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			for (auto element : buttons_) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->mouseIsOver();
				} else {
					element->mouseIsNotOver();
				}
			}
			break;
		case sf::Event::MouseButtonPressed:
			for (auto element : buttons_) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->clicked();

          if(element->getName() == "Exit") {
            getContextWindow()->getWindow().close();
          }
				}
			}
			break;
	}
}