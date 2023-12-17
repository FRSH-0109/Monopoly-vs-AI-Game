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

	void setMainBackColor(sf::Color color);
	void setMainTextColor(sf::Color color);
	void setFocusBackColor(sf::Color color);
	void setFocusTextColor(sf::Color color);

   private:
	sf::RectangleShape buttonShape_;
	sf::Text text_;
	ScreenEventType type_;

	uint btnWidth;
	uint btnHeight;

	bool isClicked_;
	bool isVisible_;

	sf::Color mainBackColor_;
	sf::Color mainTextColor_;
	sf::Color focusBackColor_;
	sf::Color focusTextColor_;
};

#endif