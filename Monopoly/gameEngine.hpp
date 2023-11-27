#include "SFML/Graphics.hpp"

class GameEngine {
private:
  sf::RenderWindow window_;
  sf::View view_;

public:
  GameEngine();
  void createWindow();
  void pollForEvents();
  void worker();

  void menu();
};