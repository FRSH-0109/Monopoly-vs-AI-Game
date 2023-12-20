#include "Player.h"

Player::Player() {
	setPositon(0);
	clearFiledOwnedId();
	setMoney(0);
	setJailStatus(0);
}

Player::Player(unsigned int money) {
	setPositon(0);
	clearFiledOwnedId();
	setMoney(money);
	setJailStatus(0);
}

unsigned int Player::getPositon() const {
	return position_;
}

void Player::setPositon(unsigned int newPosition) {
	position_ = newPosition;
}

bool Player::hasFiledOwnedId(unsigned int id) const {
	for (auto filedId : fieldsOwnedId_) {
		if (filedId == id) {
			return true;
		}
	}
	return false;
}

void Player::addFieldOwnedId(unsigned int id) {
	if (hasFiledOwnedId(id) == false) {
		fieldsOwnedId_.push_back(id);
	}
}

void Player::removeFiledOwnedId(unsigned int id) {
	// std::remove(fieldsOwnedId_.begin(), fieldsOwnedId_.end(), id);
	fieldsOwnedId_.erase(std::remove(fieldsOwnedId_.begin(), fieldsOwnedId_.end(), id), fieldsOwnedId_.end());
}

void Player::clearFiledOwnedId() {
	fieldsOwnedId_.clear();
}

void Player::setMoney(unsigned int value) {
	money_ = value;
}

unsigned int Player::getMoney() const {
	return money_;
}

void Player::addMoney(unsigned int value) {
	if(money_ + value <=  UINT_MAX)
	{
		money_ += value;
	}
}

bool Player::substractMoney(unsigned int value) {
	if(value > money_)
	{
		return false;
	}
	else{
		money_ -= value;
		return true;
	}
}

void Player::setJailStatus(unsigned int newJailStatus) {
	jailStatus_ = newJailStatus;
}

unsigned int Player::getJailStatus() const {
	return jailStatus_;
}

void Player::reduceJailStatus(){
	if(jailStatus_ != 0) {
		jailStatus_-= 1;
	}
}