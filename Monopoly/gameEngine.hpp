#include "SFML/Graphics.hpp"

class GameEngine
{
private:
    sf::RenderWindow window_;

public:
    GameEngine();
    void createWindow(); 
    void worker();
};