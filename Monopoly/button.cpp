/**
 * @file main.cpp
 *
 * @brief Source file for button class used in menu
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "button.h"

Button::Button(ScreenEventType type, std::string btnText, sf::Vector2f buttonSize, int charSize) {
	buttonShape_.setSize(buttonSize);

	btnWidth = buttonSize.x;
	btnHeight = buttonSize.y;

	text_.setString(btnText);
	text_.setCharacterSize(charSize);

	type_ = type;
}

ScreenEventType Button::getEventType() {
	return type_;
}

void Button::setFont(sf::Font& fonts) {
	text_.setFont(fonts);
}

void Button::setMainBackColor(sf::Color color) {
	mainBackColor_ = color;
	text_.setFillColor(mainTextColor_);
	buttonShape_.setFillColor(mainBackColor_);
}

void Button::setMainTextColor(sf::Color color) {
	mainTextColor_ = color;
	text_.setFillColor(mainTextColor_);
	buttonShape_.setFillColor(mainBackColor_);
}

void Button::setFocusBackColor(sf::Color color) {
	focusTextColor_ = color;
}

void Button::setFocusTextColor(sf::Color color) {
	focusTextColor_ = color;
}

void Button::setPosition(sf::Vector2f point) {
	point.x -= buttonShape_.getSize().x / 2.0f;
	point.y -= buttonShape_.getSize().y / 2.0f;
	buttonShape_.setPosition(point);

	// Center text on button:
	float xPos = (point.x + btnWidth / 2) - (text_.getLocalBounds().width / 2);
	float yPos = (point.y + btnHeight / 2.2) - (text_.getLocalBounds().height / 2);
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

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
		return true;
	}
	return false;
}

void Button::mouseIsOver() {
	text_.setFillColor(focusTextColor_);
	buttonShape_.setFillColor(focusBackColor_);
}

void Button::mouseIsNotOver() {
	text_.setFillColor(mainTextColor_);
	buttonShape_.setFillColor(mainBackColor_);
}

void Button::setIsClicked(bool state) {
	isClicked_ = state;
}

bool Button::getIsClicked() {
	return isClicked_;
}

sf::Vector2f Button::getSize() {
	return sf::Vector2f(btnWidth, btnHeight);
}

void Button::setIsVisible(bool state) {
	isVisible_ = state;
}

bool Button::getIsVisible() {
	return isVisible_;
}
