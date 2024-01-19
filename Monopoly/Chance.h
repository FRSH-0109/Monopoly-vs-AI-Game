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

/**
 * @enum ChanceType
 * @brief Enumeration representing different types of chance cards.
 */
enum ChanceType {
	MOVEMENT_TO_PROPERTY,		/**< Move to a specific property. */
	MOVEMENT_WITH_BUY_OR_PAY,	/**< Move with the option to buy or pay. */
	BANK_PAYS_YOU,				/**< Receive money from the bank. */
	GET_OUT_OF_JAIL_CARD,		/**< Receive a "Get Out of Jail Free" card. */
	MOVEMENT_SPACES,			/**< Move a certain number of spaces. */
	GO_TO_JAIL_CARD,			/**< Go directly to jail. */
	PAY_FOR_HOUSE_HOTEL,		/**< Pay for each house and hotel owned. */
	TAX_CARD,					/**< Pay a tax. */
	PAY_PLAYERS,				/**< Pay other players. */
};

/**
 * @class ChanceCard
 * @brief Represents a chance card in the monopoly game.
 */
class ChanceCard {
  private:
	unsigned int id_;             /**< Unique identifier for the chance card. */
	ChanceType type_;             /**< Type of the chance card. */
	int value_;                   /**< Value associated with the chance card. */
	sf::String text_;             /**< Text describing the action of the chance card. */
	std::string graphic_path_;    /**< Path to the graphic associated with the chance card. */
	unsigned int width_;          /**< Width of the graphic. */
	unsigned int height_;         /**< Height of the graphic. */
	sf::Texture texture_;         /**< Texture for the graphic. */
	sf::Sprite sprite_;           /**< Sprite for the graphic. */
	sf::Vector2f position_;       /**< Position of the chance card on the screen. */
	float rotation_;              /**< Rotation of the chance card graphic. */


   public:
    /**
	 * @brief Constructor for the ChanceCard class.
	 * 
	 * @param id Unique identifier for the chance card.
	 * @param type Type of the chance card.
	 * @param value Value associated with the chance card.
	 * @param text Text describing the action of the chance card.
	 * @param graphic_path Path to the graphic associated with the chance card.
	 * @param width Width of the graphic.
	 * @param height Height of the graphic.
	 * @param position Position of the chance card on the screen.
	 */
	ChanceCard(const unsigned int id,
		const ChanceType type,
		unsigned int value,
		const sf::String text,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const sf::Vector2f position);

	/**
	 * @brief Getter for the unique identifier of the chance card.
	 * 
	 * @return unsigned int Unique identifier for the chance card.
	 */
	unsigned int getId();

	/**
	 * @brief Getter for the type of the chance card.
	 * 
	 * @return ChanceType Type of the chance card.
	 */
	ChanceType getType();

	/**
	 * @brief Getter for the value associated with the chance card.
	 * 
	 * @return int Value associated with the chance card.
	 */
	int getValue();

	/**
	 * @brief Getter for the text describing the action of the chance card.
	 * 
	 * @return const sf::String& Text describing the action of the chance card.
	 */
	const sf::String getText();

	/**
	 * @brief Getter for the width of the graphic associated with the chance card.
	 * 
	 * @return unsigned int Width of the graphic.
	 */
	unsigned int getWidth();

	/**
	 * @brief Getter for the height of the graphic associated with the chance card.
	 * 
	 * @return unsigned int Height of the graphic.
	 */
	unsigned int getHeight();

	/**
	 * @brief Getter for the path to the graphic associated with the chance card.
	 * 
	 * @return const std::string& Path to the graphic.
	 */
	const std::string getGraphicPath();

	/**
	 * @brief Getter for the rotation of the chance card graphic.
	 * 
	 * @return float Rotation of the chance card graphic.
	 */
	float getRotation();

	/**
	 * @brief Getter for the sprite associated with the chance card graphic.
	 * 
	 * @return const sf::Sprite& Sprite of the chance card graphic.
	 */
	const sf::Sprite& getSprite();

	/**
	 * @brief Getter for the texture associated with the chance card graphic.
	 * 
	 * @return const sf::Texture& Texture of the chance card graphic.
	 */
	const sf::Texture& getTexture();

	/**
	 * @brief Getter for the position of the chance card on the screen.
	 * 
	 * @return const sf::Vector2f& Position of the chance card.
	 */
	const sf::Vector2f& getPosition();

	/**
	 * @brief Setter for the rotation of the chance card graphic.
	 * 
	 * @param new_roation New rotation value.
	 */
	void setRotation(float new_roation);

	/**
	 * @brief Setter for the position of the chance card on the screen.
	 * 
	 * @param pos New position value.
	 */
	void setPosition(sf::Vector2f pos);

	/**
	 * @brief Creates the sprite for the chance card using its graphic.
	 */
	void createSprite();
};

#endif