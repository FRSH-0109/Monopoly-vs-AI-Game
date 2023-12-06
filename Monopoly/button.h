#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum ButtonTypes {
	PlayButton,
	ExitButton,
};

class Button {
   public:
	Button(ButtonTypes type,
		std::string btnText,
		sf::Vector2f buttonSize,
		int charSize,
		sf::Color bgColor,
		sf::Color textColor);

	void draw(sf::RenderWindow& window);

	bool isMouseOver(sf::RenderWindow& window);
	virtual void mouseIsOver();
	virtual void mouseIsNotOver();

	void setFont(sf::Font& fonts);
	void setPosition(sf::Vector2f point);
	ButtonTypes getType();
	void setIsClicked(bool state);
	bool getIsClicked();

   private:
	void setBackColor(sf::Color color);
	void setTextColor(sf::Color color);

	sf::RectangleShape buttonShape_;
	sf::Text text_;
	ButtonTypes type_;

	uint btnWidth;
	uint btnHeight;

	bool isClicked_;
};

#endif