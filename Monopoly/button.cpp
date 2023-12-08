/**
 * @file main.cpp
 *
 * @brief Source file for button class used in menu
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "button.h"

Button::Button(ScreenEventType type,
	std::string btnText,
	sf::Vector2f buttonSize,
	int charSize,
	sf::Color bgColor,
	sf::Color textColor) {
	buttonShape_.setSize(buttonSize);
	buttonShape_.setFillColor(bgColor);

	btnWidth = buttonSize.x;
	btnHeight = buttonSize.y;

	text_.setString(btnText);
	text_.setCharacterSize(charSize);
	text_.setFillColor(textColor);

	type_ = type;
}

ScreenEventType Button::getEventType() {
	return type_;
}

void Button::setFont(sf::Font& fonts) {
	text_.setFont(fonts);
}

void Button::setBackColor(sf::Color color) {
	buttonShape_.setFillColor(color);
}

void Button::setTextColor(sf::Color color) {
	text_.setFillColor(color);
}

void Button::setPosition(sf::Vector2f point) {
	point.x -= buttonShape_.getSize().x / 2.0f;
	point.y -= buttonShape_.getSize().y / 2.0f;
	buttonShape_.setPosition(point);

	// Center text on button:
	float xPos = (point.x + btnWidth / 2) - (text_.getLocalBounds().width / 2);
	float yPos =
		(point.y + btnHeight / 2.2) - (text_.getLocalBounds().height / 2);
	text_.setPosition(xPos, yPos);
}

void Button::draw(sf::RenderWindow& window) {
	window.draw(buttonShape_);
	window.draw(text_);
}

// Check if the mouse is within the bounds of the button:
bool Button::isMouseOver(sf::RenderWindow& window) {
	int mouseX = sf::Mouse::getPosition(window).x;
	int mouseY = sf::Mouse::getPosition(window).y;

	int btnPosX = buttonShape_.getPosition().x;
	int btnPosY = buttonShape_.getPosition().y;

	int btnxPosWidth = buttonShape_.getPosition().x + btnWidth;
	int btnyPosHeight = buttonShape_.getPosition().y + btnHeight;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight &&
		mouseY > btnPosY) {
		return true;
	}
	return false;
}

void Button::mouseIsOver() {
	setBackColor(sf::Color::Black);
	setTextColor(sf::Color::Red);
}

void Button::mouseIsNotOver() {
	setBackColor(sf::Color::Red);
	setTextColor(sf::Color::Black);
}

void Button::setIsClicked(bool state) {
	isClicked_ = state;
}

bool Button::getIsClicked() {
	return isClicked_;
}

sf::Vector2f Button::getSize()
{
	return sf::Vector2f(btnWidth, btnHeight);
}
