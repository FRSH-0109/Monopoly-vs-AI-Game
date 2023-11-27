#include "mainMenu.h"

void Menu::create() {
  this->contextWindow_ = ContextWindow::GetInstance();

  if (!this->getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
    std::cout << "Font not found!\n";

  this->setFont(this->getFont());

  std::shared_ptr<Button> buttonExit(
      new Button("Exit", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
  // std::shared_ptr<Button> buttonExit = std::make_shared<Button>("Exit", {200,
  // 100}, 30, sf::Color::Green, sf::Color::Black);
  buttonExit->setFont(this->getFont());
  buttonExit->setPosition({this->contextWindow_->window_.getSize().x/2, 300});

  std::shared_ptr<Button> buttonPlay(
      new Button("Play", {200, 100}, 30, sf::Color::Red, sf::Color::Black));
  buttonPlay->setFont(this->getFont());
  buttonPlay->setPosition({this->contextWindow_->window_.getSize().x/2, 100});

  this->addButton(buttonExit);
  this->addButton(buttonPlay);
}

void Menu::setFont(sf::Font font) { this->font_ = font; }

sf::Font &Menu::getFont() { return this->font_; }

void Menu::addButton(std::shared_ptr<Button> buttonTmp) {
  std::shared_ptr<Button> button = buttonTmp;
  this->buttons_.push_back(button);
}

void Menu::draw() {
  for (auto element : this->buttons_) {
    element->draw(this->contextWindow_->window_);
  }
}

void Menu::pollForEvents(sf::Event &event) {
  switch (event.type) {
  case sf::Event::MouseMoved:
    for (auto element : this->buttons_) {
      if (element->isMouseOver(this->contextWindow_->window_)) {
        element->mouseIsOver();
      } else {
        element->mouseIsNotOver();
      }
    }
    break;
  case sf::Event::MouseButtonPressed:
    for (auto element : this->buttons_) {
      if (element->isMouseOver(this->contextWindow_->window_)) {
        if(element->getName() == "Exit")
        {
          this->contextWindow_->window_.close();
        }
        else if(element->getName() == "Play")
        {
          std::cout << "Radzik GEJ" << std::endl;
        } 
      }
    }
    break;
  }
}