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

void Button::setActiveBackColor(sf::Color color) {
	activeBackColor_ = color;
}

void Button::setActiveTextColor(sf::Color color) {
	activeTextColor_ = color;
}

void Button::setInactiveBackColor(sf::Color color) {
	inActiveBackColor_ = color;
}

void Button::setInactiveTextColor(sf::Color color) {
	inActiveTextColor_ = color;
}

void Button::setFocusBackColor(sf::Color color) {
	focusTextColor_ = color;
}

void Button::setFocusTextColor(sf::Color color) {
	focusTextColor_ = color;
}

void Button::setButtonFocus() {
	setIsFocus(true);
}

void Button::setButtonUnfocus() {
	setIsFocus(false);
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
	setButtonFocus();
}

void Button::mouseIsNotOver() {
	setButtonUnfocus();
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

void Button::setIsActive(bool state) {
	isActive_ = state;
}

bool Button::getIsActive() {
	return isActive_;
}

void Button::setIsFocus(bool state) {
	isFocus_ = state;
}

bool Button::getIsFocus() {
	return isFocus_;
}

void Button::updateColors() {
	if (isFocus_) {
		buttonShape_.setFillColor(focusBackColor_);
		text_.setFillColor(focusTextColor_);
	} else {
		if (isActive_) {
			buttonShape_.setFillColor(activeBackColor_);
			text_.setFillColor(activeTextColor_);
		} else {
			buttonShape_.setFillColor(inActiveBackColor_);
			text_.setFillColor(inActiveTextColor_);
		}
	}
}