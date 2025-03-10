/**
 * @file Button.h
 *
 * @brief Source file for handling button objects actions
 * used to communicate with user.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "Button.h"

Button::Button(ScreenEventType type, sf::String btnText, sf::Vector2f buttonSize, int charSize) {
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

void Button::setEventType(ScreenEventType event) {
	type_ = event;
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
	if (buttonShape_.getGlobalBounds().contains(
			sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
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

sf::Text& Button::getText() {
	return text_;
}

void Button::setWasReleased(bool state) {
	wasReleased_ = state;
}

bool Button::getWasReleased() {
	return wasReleased_;
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