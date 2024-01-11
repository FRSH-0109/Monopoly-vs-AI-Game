#include "Player.h"

Player::Player() {
	setPosition(0);
	clearFiledOwnedId();
	setMoney(0);
	setJailStatus(0);
}

Player::Player(unsigned int money) {
	setPosition(0);
	clearFiledOwnedId();
	setMoney(money);
	setJailStatus(0);
}

unsigned int Player::getPosition() const {
	return position_;
}

void Player::setPosition(unsigned int newPosition) {
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

std::vector<unsigned int> Player::getFiledOwnedId() const {
	return fieldsOwnedId_;
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
	if (money_ + value <= UINT_MAX) {
		money_ += value;
	}
}

bool Player::substractMoney(unsigned int value) {
	if (value > money_) {
		return false;
	} else {
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

void Player::reduceJailStatus() {
	if (jailStatus_ != 0) {
		jailStatus_ -= 1;
	}
}

void Player::setJailCards(unsigned int newJailCards) {
	jailCards_ = newJailCards;
}

unsigned int Player::getJailCards() const {
	return jailCards_;
}

void Player::setId(unsigned int newId) {
	// TODO: catch exception and tests to do
	if (newId <= 3) {
		id_ = newId;
		switch (id_) {
			case 0:
				setColor(sf::Color::Green);
				break;
			case 1:
				setColor(sf::Color::Red);
				break;
			case 2:
				setColor(sf::Color::Blue);
				break;
			case 3:
				setColor(sf::Color::Yellow);
				break;
			default:
				break;
		}
	}
}

unsigned int Player::getId() const {
	return id_;
}

void Player::setColor(sf::Color newColor) {
	color_ = newColor;
}

sf::Color Player::getColor() const {
	return color_;
}

void Player::setIsAi(bool isAiState) {
	isAi_ = isAiState;
}

bool Player::getIsAi() {
	return isAi_;
}

void Player::setAiLevel(unsigned int aiLevel) {
	// TODO excetpion
	aiLevel_ = aiLevel;
}

unsigned int Player::getAiLevel() {
	return aiLevel_;
}

void Player::setResultPlace(unsigned int place) {
	resultPlace_ = place;
}

unsigned int Player::getResultPlace() const {
	return resultPlace_;
}

void Player::createSprite() {
	const float WIDTH = 20.0;
	const float HEIGHT = 20.0;
	const std::string TEXTURE_PATH = "textures_and_fonts/textures/Pionek_monopoly.png";
	contextWindow_ = ContextWindow::GetInstance();
	if (!player_texture_.loadFromFile(TEXTURE_PATH)) {
		player_sprite_.setColor(sf::Color::Black);
	}
	player_sprite_.setTexture(player_texture_, true);
	player_sprite_.setColor(color_);
	player_sprite_.setOrigin(
		player_sprite_.getGlobalBounds().getSize() / 2.f + player_sprite_.getLocalBounds().getPosition());
	sf::Vector2u texture_dim = player_texture_.getSize();
	float scale_x = (float)WIDTH / (float)texture_dim.x;
	float scale_y = (float)HEIGHT / (float)texture_dim.y;
	const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
	player_sprite_.setScale(SCALE_VECT);
}

sf::Texture& Player::getTexture() {
	return player_texture_;
}

sf::Sprite& Player::getSprite() {
	return player_sprite_;
}

float Player::getSpriteOffsetX() const {
	return sprite_offset_x_;
}

float Player::getSpriteOffsetY() const {
	return sprite_offset_y_;
}

void Player::setSpriteOffsetX(const float offset_x) {
	if (offset_x >= 0.0 && offset_x <= 1.0) {
		sprite_offset_x_ = offset_x;
	} else {
		throw(SpriteOffsetException(offset_x));
	}
}

void Player::setSpriteOffsetY(const float offset_y) {
	if (offset_y >= 0.0 && offset_y <= 1.0) {
		sprite_offset_y_ = offset_y;
	} else {
		throw(SpriteOffsetException(offset_y));
	}
}

void Player::setSpritePosition(sf::Vector2f newPos) {
	player_sprite_.setPosition(newPos);
}
