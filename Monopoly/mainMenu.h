#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <memory>

#include "button.h"
#include "contextWindow.h"

class Menu {

private:
  ContextWindow *contextWindow_;
  sf::Font font_;
  std::vector<std::shared_ptr<Button>> buttons_;

  void setFont(sf::Font font);
  sf::Font &getFont();
  void addButton(std::shared_ptr<Button> buttonTmp);

public:
  void create();
  void draw();
  void pollForEvents(sf::Event &event);

  ContextWindow *getContextWindow();
};

#endif
