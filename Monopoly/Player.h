#ifndef PLAYER_H
#define PLAYER_H

#include <bits/stdc++.h>
#include <vector>
#include "main.h"

class Player {
	unsigned int position_;
	std::vector<unsigned int> fieldsOwnedId_;
	unsigned int money_;
	unsigned int jailStatus_;

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

};

#endif