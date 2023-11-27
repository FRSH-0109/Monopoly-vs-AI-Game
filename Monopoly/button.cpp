#include "button.h"

Button::Button(std::string btnText, sf::Vector2f buttonSize, int charSize,
               sf::Color bgColor, sf::Color textColor) {
  button.setSize(buttonSize);
  button.setFillColor(bgColor);

  btnWidth = buttonSize.x;
  btnHeight = buttonSize.y;

  text.setString(btnText);
  text.setCharacterSize(charSize);
  text.setColor(textColor);

  name_ = btnText;
}

// Pass font by reference:
void Button::setFont(sf::Font &fonts) { text.setFont(fonts); }

void Button::setBackColor(sf::Color color) { button.setFillColor(color); }

void Button::setTextColor(sf::Color color) { text.setColor(color); }

void Button::setPosition(sf::Vector2f point) {
  point.x -= button.getSize().x/2;
  point.y -= button.getSize().y/2;
  button.setPosition(point);

  // Center text on button:
  float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
  float yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
  text.setPosition(xPos, yPos);
}

void Button::draw(sf::RenderWindow &window) {
  window.draw(this->button);
  window.draw(this->text);
}

// Check if the mouse is within the bounds of the button:
bool Button::isMouseOver(sf::RenderWindow &window) {
  int mouseX = sf::Mouse::getPosition(window).x;
  int mouseY = sf::Mouse::getPosition(window).y;

  int btnPosX = button.getPosition().x;
  int btnPosY = button.getPosition().y;

  int btnxPosWidth = button.getPosition().x + btnWidth;
  int btnyPosHeight = button.getPosition().y + btnHeight;

  if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight &&
      mouseY > btnPosY) {
    return true;
  }
  return false;
}

void Button::mouseIsOver() { this->setBackColor(sf::Color::Black);this->setTextColor(sf::Color::Red); }

void Button::mouseIsNotOver() { this->setBackColor(sf::Color::Red);this->setTextColor(sf::Color::Black); }

std::string& Button::getName() {return this->name_;}