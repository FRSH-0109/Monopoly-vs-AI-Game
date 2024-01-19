/**
 * @file Player.h
 *
 * @brief Implementation file for Player class and AI Player class,
 * containing data and methods for a player in a Monopoly game.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include "AiAdapter.h"
#include "ContextWindow.h"
#include "Tinyann.h"
#include "Tinyneat.h"
#include "main.h"

/**
 * @class Player
 *
 * @brief Represents a player in a Monopoly game.
 */
class Player {
	bool is_ai_;								/**< Indicates whether the player is controlled by AI. */
	unsigned int ai_level_;						/**< AI level of the player. Useless if player is human. */
	unsigned int id_;							/**< Unique identifier for the player. */
	sf::Color color_;							/**< Color associated with the player. */
	unsigned int position_;						/**< Current position of the player on the game board. */
	std::vector<unsigned int> fields_owned_id_; /**< List of property IDs owned by the player. */
	unsigned int money_;						/**< Amount of money the player has. */
	unsigned int jail_status_;					/**< Jail status of the player. */
	sf::Texture player_texture_;				/**< Texture for the player's sprite. */
	sf::Sprite player_sprite_;					/**< Sprite representing the player on the game board. */
	float sprite_offset_x_;						/**< Offset of the player sprite along the X-axis. */
	float sprite_offset_y_;						/**< Offset of the player sprite along the Y-axis. */
	unsigned int jail_cards_;					/**< Number of jail cards the player has. */
	unsigned int result_place_;					/**< Final result place of the player in the game. */

   public:
	/** Default constructor for Player class. */
	Player();

	/**
	 * Constructor for Player class with initial money.
	 *
	 * @param money Initial amount of money for the player.
	 */
	Player(unsigned int money);

	/** Get the current position of the player. */
	unsigned int getPosition() const;

	/**
	 * Set the current position of the player.
	 *
	 * @param new_position New position for the player.
	 */
	void setPosition(unsigned int new_position);

	/**
	 * Check if the player owns a property with the specified ID.
	 *
	 * @param id ID of the property to check.
	 * @return True if the player owns the property, false otherwise.
	 */
	bool hasFieldOwnedId(unsigned int id) const;

	/** Get a vector of property IDs owned by the player. */
	std::vector<unsigned int> getFieldOwnedId() const;

	/**
	 * Add a property with the specified ID to the list of properties owned by the player.
	 *
	 * @param id ID of the property to add.
	 */
	void addFieldOwnedId(unsigned int id);

	/**
	 * Remove a property with the specified ID from the list of properties owned by the player.
	 *
	 * @param id ID of the property to remove.
	 */
	void removeFieldOwnedId(unsigned int id);

	/** Clear the list of properties owned by the player. */
	void clearFieldOwnedId();

	/**
	 * Set the amount of money the player has.
	 *
	 * @param value New amount of money for the player.
	 */
	void setMoney(unsigned int value);

	/** Get the current amount of money the player has. */
	unsigned int getMoney() const;

	/**
	 * Add a specified amount of money to the player's balance.
	 *
	 * @param value Amount of money to add.
	 */
	void addMoney(unsigned int value);

	/**
	 * Subtract a specified amount of money from the player's balance.
	 *
	 * @param value Amount of money to subtract.
	 * @return True if the player had enough money and the subtraction was successful, false otherwise.
	 */
	bool substractMoney(unsigned int value);

	/**
	 * Set the jail status of the player.
	 *
	 * @param new_jail_status New jail status for the player.
	 */
	void setJailStatus(unsigned int new_jail_status);

	/** Get the current jail status of the player. */
	unsigned int getJailStatus() const;

	/**
	 * Set the number of jail cards the player has.
	 *
	 * @param new_jail_cards New number of jail cards for the player.
	 */
	void setJailCards(unsigned int new_jail_cards);

	/** Get the current number of jail cards the player has. */
	unsigned int getJailCards() const;

	/** Reduce the jail status of the player by one. */
	void reduceJailStatus();

	/**
	 * Set the unique identifier for the player.
	 *
	 * @param new_id New unique identifier for the player.
	 */
	void setId(unsigned int new_id);

	/** Get the unique identifier of the player. */
	unsigned int getId() const;

	/**
	 * Set the color associated with the player.
	 *
	 * @param new_color New color for the player.
	 */
	void setColor(sf::Color new_color);

	/** Get the color associated with the player. */
	sf::Color getColor() const;

	/**
	 * Set the AI status of the player.
	 *
	 * @param new_is_ai New AI status for the player.
	 */
	void setIsAi(bool new_is_ai);

	/** Get the AI status of the player. */
	bool getIsAi() const;

	/**
	 * Set the AI level of the player.
	 *
	 * @param ai_level New AI level for the player.
	 */
	void setAiLevel(unsigned int ai_level);

	/** Get the AI level of the player. */
	unsigned int getAiLevel() const;

	/** Set the final result place of the player in the game. */
	void setResultPlace(unsigned int place);

	/** Get the final result place of the player in the game. */
	unsigned int getResultPlace() const;

	/** Create the sprite for the player. */
	void createSprite();

	/** Get the texture of the player's sprite. */
	sf::Texture& getTexture();

	/** Get the sprite representing the player on the game board. */
	sf::Sprite& getSprite();

	/** Get the offset of the player sprite along the X-axis. */
	float getSpriteOffsetX() const;

	/** Get the offset of the player sprite along the Y-axis. */
	float getSpriteOffsetY() const;

	/**
	 * Set the offset of the player sprite along the X-axis.
	 *
	 * @param offset_x New offset value.
	 */
	void setSpriteOffsetX(const float offset_x);

	/**
	 * Set the offset of the player sprite along the Y-axis.
	 *
	 * @param offset_y New offset value.
	 */
	void setSpriteOffsetY(const float offset_y);

	/**
	 * Set the position of the player sprite.
	 *
	 * @param new_pos New position for the player sprite.
	 */
	void setSpritePosition(sf::Vector2f new_pos);

	/**
	 * Get the AI adapter (virtual function, needs to be overridden by derived classes).
	 *
	 * @return Reference to the AI adapter.
	 */
	virtual AiAdapter& getAdapter(){};

	/**
	 * Get the Neural Network of a player (virtual function, needs to be overridden by derived classes).
	 *
	 * @return Reference to the neuralnet class from a tinyai library.
	 */
	virtual ann::neuralnet& getNeuralNetwork(){};

	/**
	 * Make a buying decision (virtual function, needs to be overridden by derived classes).
	 *
	 * @param index Index of the property to consider.
	 * @return BuyDecision object representing the decision.
	 */
	virtual BuyDecision decideBuy(unsigned int index);

	/**
	 * Make a jail decision (virtual function, needs to be overridden by derived classes).
	 *
	 * @return JailDecision object representing the decision.
	 */
	virtual JailDecision decideJail();

	/**
	 * Make a mortgage decision (virtual function, needs to be overridden by derived classes).
	 *
	 * @param index Index of the property to consider.
	 * @return Decision object representing the mortgage decision.
	 */
	virtual Decision decideMortgage(unsigned int index);

	/**
	 * Make an unmortgage decision (virtual function, needs to be overridden by derived classes).
	 *
	 * @param index Index of the property to consider.
	 * @return Decision object representing the unmortgage decision.
	 */
	virtual Decision decideUnmortgage(unsigned int index);

	/**
	 * Make a decision for auction bidding (virtual function, needs to be overridden by derived classes).
	 *
	 * @param price Current price in the auction.
	 * @return The bid amount decided by the player.
	 */
	virtual unsigned int decideAuctionBid(unsigned int price);

	/**
	 * Make a decision for building a house (virtual function, needs to be overridden by derived classes).
	 *
	 * @return The index of the property on which to build a house.
	 */
	virtual unsigned int decideBuildHouse();

	/**
	 * Make a decision for selling a house (virtual function, needs to be overridden by derived classes).
	 *
	 * @return The index of the property from which to sell a house.
	 */
	virtual unsigned int decideSellHouse();

	/**
	 * Make a decision for offering a trade (virtual function, needs to be overridden by derived classes).
	 *
	 * @return Decision object representing the trade offer.
	 */
	virtual Decision decideOfferTrade();

	/**
	 * Make a decision for accepting a trade (virtual function, needs to be overridden by derived classes).
	 *
	 * @return Decision object representing the acceptance of the trade.
	 */
	virtual Decision decideAcceptTrade();
};

/**
 * @class AiPlayer
 *
 * @brief Represents an AI player in a Monopoly game, inheriting from Player.
 */
class AiPlayer : public Player {
	AiAdapter adapter_;				/**< AI adapter associated with the AI player. */
	ann::neuralnet neural_network_; /**< Neural network for decision-making. */

   public:
	/** Default constructor for AiPlayer class. */
	AiPlayer();

	/**
	 * Constructor for AiPlayer class with initial money.
	 *
	 * @param money Initial amount of money for the AI player.
	 */
	AiPlayer(unsigned int money);

	/**
	 * Constructor for AiPlayer class with initial money and neural_network.
	 *
	 * @param money Initial amount of money for the AI player.
	 * @param n Neural network used by a Player.
	 */
	AiPlayer(unsigned int money, ann::neuralnet nn);

	/** Get the AI adapter associated with the AI player. */
	AiAdapter& getAdapter();

	/** Get the neural network associated with the AI player. */
	ann::neuralnet& getNeuralNetwork();

	/** Set the neura network used by the AI player. */
	void setNeuralNetwork(ann::neuralnet new_nn);

	/**
	 * Make a buying decision for the AI player (override from base class).
	 *
	 * @param index Index of the property to consider.
	 * @return BuyDecision object representing the decision.
	 */
	BuyDecision decideBuy(unsigned int index) override;

	/**
	 * Make a jail decision for the AI player (override from base class).
	 *
	 * @return JailDecision object representing the decision.
	 */
	JailDecision decideJail() override;

	/**
	 * Make a mortgage decision for the AI player (override from base class).
	 *
	 * @param index Index of the property to consider.
	 * @return Decision object representing the mortgage decision.
	 */
	Decision decideMortgage(unsigned int index) override;

	/**
	 * Make an unmortgage decision for the AI player (override from base class).
	 *
	 * @param index Index of the property to consider.
	 * @return Decision object representing the unmortgage decision.
	 */
	Decision decideUnmortgage(unsigned int index) override;

	/**
	 * Make a decision for auction bidding for the AI player (override from base class).
	 *
	 * @param price Current price in the auction.
	 * @return The bid amount decided by the AI player.
	 */
	unsigned int decideAuctionBid(unsigned int price) override;

	/**
	 * Make a decision for building a house for the AI player (override from base class).
	 *
	 * @return The index of the property on which the AI player decides to build a house.
	 */
	unsigned int decideBuildHouse() override;

	/**
	 * Make a decision for selling a house for the AI player (override from base class).
	 *
	 * @return The index of the property from which the AI player decides to sell a house.
	 */
	unsigned int decideSellHouse() override;

	/**
	 * Make a decision for offering a trade for the AI player (override from base class).
	 *
	 * @return Decision object representing the trade offer.
	 */
	Decision decideOfferTrade() override;

	/**
	 * Make a decision for accepting a trade for the AI player (override from base class).
	 *
	 * @return Decision object representing the acceptance of the trade.
	 */
	Decision decideAcceptTrade() override;
};

#endif