#include "Field.h"

// HouseException(unsigned int houses) : invalid_number_(houses){};
// HouseException(const HouseException& e) throw() : invalid_number_(e.invalid_number_){};

Field::Field(const unsigned int id,
	const FieldType type,
	const std::string name,
	const std::string graphic_path,
	const unsigned int width,
	const unsigned int height,
	const float rotation,
	const sf::Vector2i position)
	: id_(id),
	  type_(type),
	  name_(name),
	  graphic_path_(graphic_path),
	  width_(width),
	  height_(height),
	  rotation_(rotation),
	  position_(position) {
	std::cout << "Field constructor" << std::endl;
}

ContextWindow* Field::getContextWindow() {
	contextWindow_ = ContextWindow::GetInstance();
	return contextWindow_;
}

const unsigned int HouseException::getInvalidNumber() {
	return invalid_number_;
};

const unsigned int Field::getId() {
	return id_;
};

const FieldType Field::getType() {
	return type_;
};

const std::string Field::getName() {
	return name_;
};

const std::string Field::getGraphicPath() {
	return graphic_path_;
};

const unsigned int Field::getWidth() {
	return width_;
};

const unsigned int Field::getHeight() {
	return height_;
};

const float Field::getRotation() {
	return rotation_;
};

const sf::Sprite& Field::getSprite() {
	return sprite_;
}

const sf::Texture& Field::getTexture() {
	return texture_;
}

const sf::Vector2i& Field::getPosition() {
	return position_;
}

void Field::createSprite() {
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

void Field::setHeight(unsigned int new_height) {
	if (contextWindow_->window_.getSize().y >= new_height && new_height != 0) {
		height_ = new_height;
	} else {
		throw(DimensionException(new_height));
	}
};

void Field::setWidth(unsigned int new_width) {
	if (contextWindow_->window_.getSize().x >= new_width && new_width != 0) {
		width_ = new_width;
	} else {
		throw(DimensionException(new_width));
	}
};

void Field::setRotation(float new_roation) {
	if (new_roation > 0.0 && new_roation < 360.0) {
		rotation_ = new_roation;
	} else {
		throw(RotationException(new_roation));
	}
};

void Field::setPosition(sf::Vector2i pos) {
	position_ = pos;
}

const unsigned int PropertyField::getPrice() {
	return price_;
};

const std::vector<unsigned int> PropertyField::getGroupMembers() {
	return group_members_;
};

const unsigned int PropertyField::getMortage() {
	return mortage_;
};

const bool PropertyField::getIsMortaged() {
	return is_mortaged_;
};
const unsigned int PropertyField::getUnmortageValue() {
	return static_cast<int>(round(1.1 * mortage_));
};

std::shared_ptr<Player>  PropertyField::getOwner() {
	return owner_;
};

sf::RectangleShape PropertyField::getOwnerFlag() {
	return owner_flag_;
}

void PropertyField::setIsMortaged(bool new_state) {
	is_mortaged_ = new_state;
};

void PropertyField::setOwner(std::shared_ptr<Player> new_owner_ptr) {
	owner_ = new_owner_ptr;
};

void PropertyField::resetDefault() {
	PropertyField::setIsMortaged(false);
	PropertyField::setOwner(nullptr);
};

void PropertyField::createFlagSprite() {
	float flag_x_pos;
	float flag_y_pos;
	unsigned int field_id = this->getId();
	float field_width = (float)this->getWidth();
	float field_height = (float)this->getHeight();
	float field_rotation = this->getRotation();
	const sf::Vector2i& field_pos = this->getPosition();
	owner_flag_ = sf::RectangleShape(sf::Vector2f(field_width - 20.0f, field_height * 0.1f));

	if (field_id >= 0 && field_id <= 10) {
		flag_x_pos = (float)field_pos.x + 10.0f;
		flag_y_pos = (float)field_pos.y + field_height + 8.0f;
	} else if (field_id > 10 && field_id <= 20) {
		flag_x_pos = (float)field_pos.x - field_height - 8.0f;
		flag_y_pos = (float)field_pos.y + 10.0f;
	} else if (field_id > 20 && field_id <= 30) {
		flag_x_pos = (float)field_pos.x - 10.0f;
		flag_y_pos = (float)field_pos.y - field_height - 8.0f;
	} else if (field_id > 30 && field_id <= 40) {
		flag_x_pos = (float)field_pos.x + field_height + 8.0f;
		flag_y_pos = (float)field_pos.y - 10.0f;
	}

	owner_flag_.setRotation(field_rotation);
	owner_flag_.setOutlineThickness(2.0f);
	owner_flag_.setOutlineColor(sf::Color::Black);
	owner_flag_.setPosition(sf::Vector2f(flag_x_pos, flag_y_pos));

	if (owner_ != nullptr) {
		owner_flag_.setFillColor(owner_->getColor());
	}
};

const std::map<StreetTiers, unsigned int> StreetField::getRentValues() {
	return rent_values_;
};

const unsigned int StreetField::getHouseNumber() {
	return house_number_;
};

const bool StreetField::getIsHotel() {
	return is_hotel_;
}

void StreetField::setHouseNumber(unsigned int new_house_number) {
	if (new_house_number <= 4) {
		house_number_ = new_house_number;
	} else {
		throw(HouseException(new_house_number));
	}
};

void StreetField::setIsHotel(bool new_state) {
	is_hotel_ = new_state;
};

void StreetField::resetDefault() {
	StreetField::setHouseNumber(0);
	StreetField::setIsHotel(false);
	StreetField::setIsMortaged(false);
	StreetField::setOwner(nullptr);
};

const std::map<StationTiers, unsigned int> StationField::getRentValues() {
	return rent_values_;
};

const std::map<UtilityTiers, unsigned int> UtilityField::getRentMultipliers() {
	return rent_multipliers_;
};

const unsigned int TaxField::getTaxValue() {
	return tax_value_;
};
