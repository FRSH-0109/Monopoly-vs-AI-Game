#pragma once

// #include <tinyai/src/tinyneat.hpp>
// #include <tinyai/src/tinyann.hpp>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "contextWindow.h"
#include "Field.h"
// #include "AiAdapter.h"
#include "main.h"

enum Decision {
	YES,
	NO
};

enum JailDecision {
	ROLL,
	PAY,
	CARD
};

enum BuyDecision {
	BUY,
	AUCTION
};

class Player {
	ContextWindow* contextWindow_;
	bool isAi_;
	unsigned int aiLevel_;
	unsigned int id_;
	sf::Color color_;
	unsigned int position_;
	std::vector<unsigned int> fieldsOwnedId_;
	unsigned int money_;
	unsigned int jailStatus_;
	sf::Texture player_texture_;
	sf::Sprite player_sprite_;
	float sprite_offset_x_;
	float sprite_offset_y_;
	unsigned int jailCards_;
	unsigned int resultPlace_;

   public:
	Player();
	Player(unsigned int money);
	unsigned int getPosition() const;
	void setPosition(unsigned int newPosition);
	bool hasFiledOwnedId(unsigned int id) const;
	std::vector<unsigned int> getFiledOwnedId() const;
	void addFieldOwnedId(unsigned int id);
	void removeFiledOwnedId(unsigned int id);
	void clearFiledOwnedId();
	void setMoney(unsigned int value);
	unsigned int getMoney() const;
	void addMoney(unsigned int value);
	bool substractMoney(unsigned int value);
	void setJailStatus(unsigned int newJailStatus);
	unsigned int getJailStatus() const;
	void setJailCards(unsigned int newJailCards);
	unsigned int getJailCards() const;
	void reduceJailStatus();
	void setId(unsigned int newId);
	unsigned int getId() const;
	void setColor(sf::Color newColor);
	sf::Color getColor() const;
	void setIsAi(bool isAiState);
	bool getIsAi();
	void setAiLevel(unsigned int aiLevel);
	unsigned int getAiLevel();
	void setResultPlace(unsigned int place);
	unsigned int getResultPlace() const;
	void createSprite();
	sf::Texture& getTexture();
	sf::Sprite& getSprite();
	float getSpriteOffsetX() const;
	float getSpriteOffsetY() const;
	void setSpriteOffsetX(const float offset_x);
	void setSpriteOffsetY(const float offset_y);
	void setSpritePosition(sf::Vector2f newPos);

	virtual const unsigned int getTest() { return 0; };
	virtual BuyDecision decideBuy(unsigned int index);
	virtual JailDecision decideJail();
	virtual Decision decideMortgage(unsigned int index);
	virtual Decision decideUnmortgage(unsigned int index);
	virtual unsigned int decideAuctionBid(PropertyField& field);
	virtual unsigned int decideBuildHouse(StreetField& field);
	virtual unsigned int decideSellHouse(StreetField& field);
	virtual Decision decideOfferTrade();
	virtual Decision decideAcceptTrade();
};

class AiPlayer : public Player {
	AiAdapter adapter_;
	ann::neuralnet neural_network_;
	unsigned int test_ = 3;

   public:
	AiPlayer() : Player(){};
	AiPlayer(unsigned int money, ann::neuralnet& neural_network)
		: Player(money),
		neural_network_(neural_network)
		{};
	AiAdapter& getAdapter();
	const unsigned int getTest() { return test_; };
	BuyDecision decideBuy(unsigned int index);
	JailDecision decideJail();
	Decision decideMortgage(unsigned int index);
	Decision decideUnmortgage(unsigned int index);
	unsigned int decideAuctionBid(PropertyField& field);
	unsigned int decideBuildHouse(StreetField& field);
	unsigned int decideSellHouse(StreetField& field);
	Decision decideOfferTrade();
	Decision decideAcceptTrade();
};
