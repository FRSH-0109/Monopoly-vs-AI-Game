#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include "main.h"

enum ChanceType
{
    MovementToProperty,
    MovementWithBuyOrPay,
    BankPaysYou,
    GetOutOfJailCard,
    MovementSpaces,
    GoToJail,
    PayForHouseHotel,
    Tax,
    PayPlayers,
};

class ChanceCard {
    private:
        unsigned int id_;
        ChanceType type_;
        unsigned int value_;
        std::string text_;
        std::string graphic_path_;
        unsigned int width_;
        unsigned int height_;
        sf::Texture texture_;
        sf::Sprite sprite_;
        sf::Vector2f position_;
        float rotation_;

    public:
    ChanceCard(const unsigned int id,
		const ChanceType type,
        unsigned int value,
		const std::string text,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const sf::Vector2f position);

    const unsigned int getId();
    const ChanceType getType();
    const unsigned int getValue();
    const std::string getText();
    const unsigned int getWidth();
    const unsigned int getHeight();

    const std::string getGraphicPath();
    const float getRotation();
	const sf::Sprite& getSprite();
	const sf::Texture& getTexture();
	const sf::Vector2f& getPosition();
    void setRotation(float new_roation);
	void setPosition(sf::Vector2f pos);
    
    void createSprite();
};