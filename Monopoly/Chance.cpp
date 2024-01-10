#include "Chance.h"

ChanceCard::ChanceCard(const unsigned int id,
	const ChanceType type,
    unsigned int value,
    const std::string text,
    const std::string graphic_path,
    const unsigned int width,
    const unsigned int height,
    const sf::Vector2f position)
	: id_(id),
	  type_(type),
      value_(value),
	  text_(text),
	  graphic_path_(graphic_path),
	  width_(width),
	  height_(height),
	  position_(position) {
	rotation_ = 0;
}


const unsigned int ChanceCard::getId() {
	return id_;
};

const ChanceType ChanceCard::getType() {
	return type_;
};

const std::string ChanceCard::getText() {
	return text_;
};

const std::string ChanceCard::getGraphicPath() {
	return graphic_path_;
};

const unsigned int ChanceCard::getWidth() {
	return width_;
};

const unsigned int ChanceCard::getHeight() {
	return height_;
};

const float ChanceCard::getRotation() {
	return rotation_;
};

const sf::Sprite& ChanceCard::getSprite() {
	return sprite_;
}

const sf::Texture& ChanceCard::getTexture() {
	return texture_;
}

const sf::Vector2f& ChanceCard::getPosition() {
	return position_;
}

void ChanceCard::createSprite() {
	if (!texture_.loadFromFile(graphic_path_)) {
		sprite_.setColor(sf::Color::Green);
	}
	sprite_.setTexture(texture_, true);
	sf::Vector2u texture_dim = texture_.getSize();
	float scale_x = (float)this->width_ / (float)texture_dim.x;
	float scale_y = (float)this->height_ / (float)texture_dim.y;
	const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
	sprite_.setScale(SCALE_VECT);
	sprite_.setPosition(position_.x, position_.y);
	sprite_.setRotation(rotation_);
}