#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Button {
   public:
	Button(std::string btnText,
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
	std::string& getName();
	virtual void clicked();

   private:

	void setBackColor(sf::Color color);
	void setTextColor(sf::Color color);

	sf::RectangleShape buttonShape_;
	sf::Text text_;
	std::string name_;

	uint btnWidth;
	uint btnHeight;
};

class ButtonExit: public Button {
	public:
	using Button::Button;
	void clicked() override;
};

class ButtonPlay: public Button {
	public:
	using Button::Button;
	void clicked() override;
};

#endif