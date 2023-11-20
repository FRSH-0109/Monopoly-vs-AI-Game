#include "gameEngine.hpp"

GameEngine::GameEngine()
{
    
}

void GameEngine::createWindow()
{
    this->window_.create(sf::VideoMode(), "MonopolyVsAI", sf::Style::Fullscreen);
} 

void GameEngine::worker()
{
    while (this->window_.isOpen())
    {
        sf::Event event;
        while (this->window_.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:

                break;
            }
            // if (event.type == sf::Event::Closed)
            //     this->window_.close();
        }

        this->window_.clear();
        this->window_.display();
    }
}
