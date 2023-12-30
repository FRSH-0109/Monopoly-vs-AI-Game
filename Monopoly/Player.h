#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "contextWindow.h"
#include "main.h"

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

   public:
	Player();
	Player(unsigned int money);
	unsigned int getPositon() const;
	void setPositon(unsigned int newPosition);
	bool hasFiledOwnedId(unsigned int id) const;
	void addFieldOwnedId(unsigned int id);
	void removeFiledOwnedId(unsigned int id);
	void clearFiledOwnedId();
	void setMoney(unsigned int value);
	unsigned int getMoney() const;
	void addMoney(unsigned int value);
	bool substractMoney(unsigned int value);
	void setJailStatus(unsigned int newJailStatus);
	unsigned int getJailStatus() const;
	void reduceJailStatus();
	void setId(unsigned int newId);
	unsigned int getId() const;
	void setColor(sf::Color newColor);
	sf::Color getColor() const;
	void setIsAi(bool isAiState);
	bool getIsAi();
	void setAiLevel(unsigned int aiLevel);
	unsigned int getAiLevel();
	void createSprite();
	sf::Texture& getTexture();
	sf::Sprite& getSprite();
	float getSpriteOffsetX() const;
	float getSpriteOffsetY() const;
	void setSpriteOffsetX(const float offset_x);
	void setSpriteOffsetY(const float offset_y);
	void setSpritePosition(sf::Vector2f newPos);
};
