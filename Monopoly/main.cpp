#include "SFML/Graphics.hpp"

#include "gameEngine.hpp"

int main() {
  GameEngine gameEngine;

  gameEngine.createWindow();

  gameEngine.worker();

  return 0;
}