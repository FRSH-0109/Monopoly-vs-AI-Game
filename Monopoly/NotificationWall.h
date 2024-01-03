#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class NotificationWall 
{
    private:
    unsigned int MAX_NOTIFICATIONS_ = 10;
    sf::Vector2f DRAW_OFFSET_ = sf::Vector2f(300,300);
    unsigned int Y_STEP_OFFSET_ = 25;
    std::vector<std::shared_ptr<sf::Text>> wallTexts_;
    unsigned int FONT_SIZE_ = 25;
    sf::Font font_;

    public:
    NotificationWall();
    void clearWall();
    std::vector<std::shared_ptr<sf::Text>>& getWall();
    void addToWall(std::string text);
    unsigned int getFontSize() const;
    void setFont(sf::Font font);
	sf::Font& getFont();
    
};