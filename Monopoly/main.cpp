#include "SFML/Graphics.hpp"
#include "gameEngine.h"

int main() {
  GameEngine gameEngine(30, 1000, 1000);

  gameEngine.worker();

  return 0;
}