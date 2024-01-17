/**
 * @file Chance.h
 *
 * @brief Header file for monopoly game chance cards,
 * their types, actions...
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef CHANCE_H
#define CHANCE_H

#include <SFML/Graphics.hpp>

#include <string>
#include "main.h"

enum ChanceType {
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
	int value_;
	sf::String text_;
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
		const sf::String text,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const sf::Vector2f position);

	unsigned int getId();
	ChanceType getType();
	int getValue();
	const sf::String getText();
	unsigned int getWidth();
	unsigned int getHeight();
	const std::string getGraphicPath();
	float getRotation();
	const sf::Sprite& getSprite();
	const sf::Texture& getTexture();
	const sf::Vector2f& getPosition();
	void setRotation(float new_roation);
	void setPosition(sf::Vector2f pos);

	void createSprite();
};

#endif