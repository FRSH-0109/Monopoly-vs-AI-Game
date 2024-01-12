/**
 * @file Button.h
 *
 * @brief Header file for handling button objects actions
 * used to communicate with user.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "main.h"

class Button {
   public:
	Button(ScreenEventType type, std::string btnText, sf::Vector2f buttonSize, int charSize);

	void draw(sf::RenderWindow& window);

	bool isMouseOver(sf::RenderWindow& window);
	virtual void mouseIsOver();
	virtual void mouseIsNotOver();
	ScreenEventType getEventType();

	void setFont(sf::Font& fonts);
	void setPosition(sf::Vector2f point);
	void setIsClicked(bool state);
	bool getIsClicked();
	sf::Vector2f getSize();
	void setIsVisible(bool state);
	bool getIsVisible();
	void setIsActive(bool state);
	bool getIsActive();
	void setIsFocus(bool state);
	bool getIsFocus();
	void setWasReleased(bool state);
	bool getWasReleased();
	sf::Text& getText();

	void setActiveBackColor(sf::Color color);
	void setActiveTextColor(sf::Color color);
	void setInactiveBackColor(sf::Color color);
	void setInactiveTextColor(sf::Color color);
	void setFocusBackColor(sf::Color color);
	void setFocusTextColor(sf::Color color);
	void setButtonFocus();
	void setButtonUnfocus();
	void updateColors();

   private:
	sf::RectangleShape buttonShape_;
	sf::Text text_;
	ScreenEventType type_;

	uint btnWidth;
	uint btnHeight;

	bool isClicked_;
	bool isVisible_;
	bool isActive_;
	bool isFocus_;
	bool wasReleased_;

	sf::Color activeBackColor_;
	sf::Color activeTextColor_;
	sf::Color inActiveBackColor_;
	sf::Color inActiveTextColor_;
	sf::Color focusBackColor_;
	sf::Color focusTextColor_;
};

#endif