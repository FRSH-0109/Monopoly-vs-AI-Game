#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "main.h"

class Button {
   public:
	Button(ScreenEventType type,
		std::string btnText,
		sf::Vector2f buttonSize,
		int charSize,
		sf::Color bgColor,
		sf::Color textColor);

	void draw(sf::RenderWindow& window);

	bool isMouseOver(sf::RenderWindow& window);
	virtual void mouseIsOver();
	virtual void mouseIsNotOver();
	ScreenEventType getEventType();

	void setFont(sf::Font& fonts);
	void setPosition(sf::Vector2f point);
	void setIsClicked(bool state);
	bool getIsClicked();

   private:
	void setBackColor(sf::Color color);
	void setTextColor(sf::Color color);

	sf::RectangleShape buttonShape_;
	sf::Text text_;
	ScreenEventType type_;

	uint btnWidth;
	uint btnHeight;

	bool isClicked_;
};

#endif