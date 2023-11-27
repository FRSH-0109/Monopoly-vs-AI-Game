#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button {
public:
  Button(std::string btnText, sf::Vector2f buttonSize, int charSize,
         sf::Color bgColor, sf::Color textColor);

  // Pass font by reference:
  void setFont(sf::Font &fonts);
  void setBackColor(sf::Color color);
  void setTextColor(sf::Color color);
  void setPosition(sf::Vector2f point);
  void draw(sf::RenderWindow &window);

  // Check if the mouse is within the bounds of the button:
  bool isMouseOver(sf::RenderWindow &window);
  void mouseIsOver();
  void mouseIsNotOver();
  std::string& getName();

private:
  sf::RectangleShape button;
  sf::Text text;
  std::string name_;

  int btnWidth;
  int btnHeight;
};

#endif